#include <fstream>
#include <vector>

#include "base64.hpp"
#include "common/log.h"
#include "sampling.h"
#include "llama.h"
#include "ggml.h"
#include "common.h"
#include "arg.h"
#include "console.h"
#include "src/llama-context.h"

struct llava_context {
 struct clip_ctx * ctx_clip = NULL;
 struct llama_context * ctx_llama = NULL;
 struct llama_model * model = NULL;
};

static void print_usage(int argc, char ** argv) {
  (void) argc;

  LOG("\nexample usage:\n");
  LOG("\n  text generation:     %s -m your_model.gguf -p \"I believe the meaning of life is\" -n 128\n", argv[0]);
  LOG("\n  chat (conversation): %s -m your_model.gguf -p \"You are a helpful assistant\" -cnv\n", argv[0]);
  LOG("\n");
}

int main(int argc, char** argv) {
  static common_params* g_params;
  common_params params;
  g_params = &params;
  if (!common_params_parse(argc, argv, params, LLAMA_EXAMPLE_MAIN, print_usage)) {
      return 1;
  }

  std::string llm_model_name_ = "/userdata/MagicBox/config/qwen2.5-1.5b-instruct-q5_k_m.gguf";
  params.model = llm_model_name_;
  params.cpuparams.n_threads = 8;
  params.sampling.temp = 0.5;
  params.n_predict = 256;
  std::string value = "/userdata/MagicBox/config/system_prompt.txt";
  std::ifstream file(value);
  if (!file) {
      throw std::runtime_error(string_format("error: failed to open file '%s'\n", value.c_str()));
  }
  // store the external file name in params
  params.prompt_file = value;
  std::copy(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>(), back_inserter(params.prompt));
  if (!params.prompt.empty() && params.prompt.back() == '\n') {
      params.prompt.pop_back();
  }

  common_init();

  auto & sparams = params.sampling;

  // save choice to use color for later
  // (note for later: this is a slightly awkward choice)
  console::init(params.simple_io, params.use_color);
  atexit([]() { console::cleanup(); });

  llama_backend_init();
  llama_numa_init(params.numa);

  llama_model * model = nullptr;
  llama_context * ctx = nullptr;
  common_sampler * smpl = nullptr;

  // load the model and apply lora adapter, if any
  LOG_INF("%s: load the model and apply lora adapter, if any\n", __func__);
  common_init_result llama_init = common_init_from_params(params);

  model = llama_init.model.get();
  ctx = llama_init.context.get();

  const llama_vocab * vocab = llama_model_get_vocab(model);
  const bool add_bos = llama_vocab_get_add_bos(vocab) && !params.use_jinja;
  if (!llama_model_has_encoder(model)) {
      GGML_ASSERT(!llama_vocab_get_add_eos(vocab));
  }

  for(int i = 0; i < 3; i++){
    std::vector<std::string> warmup_texts = {
      "你好，这是第一段预热文本。",
      "模型预热第二段，用于激活所有 kernel。",
      "最后一段，确保 KV cache 已经填满。"
    };

    for (auto& text : warmup_texts) {
      llama_token tokens[256];
      int32_t n_text_chars = static_cast<int32_t>(text.size());
      int32_t n_tokens = llama_tokenize(&model->vocab, text.c_str(), n_text_chars,
                                          tokens, 256, true, true);

      llama_batch batch = llama_batch_get_one(tokens, n_tokens);
      llama_decode(ctx, batch);
    }
  }
}