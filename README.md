English| [简体中文](./README_cn.md)

Getting Started with qwen_llm Project
=======

# Feature Introduction

The qwen_llm package is a large language model functional package that adapts MagicBox dialogue functionality based on [llama.cpp](https://github.com/ggml-org/llama.cpp).

- Pure Language Model (LLM): Supports setting system prompts, accepting prompt text input, and generating text-based dialogue output. The text input can be configured via parameters or dynamically controlled at runtime through string msg topic messages. The generated text output is published via string msg topic messages.

- If you want to use the VLM function, please refer to the hobot_llamacpp project. Currently, this project is only compatible with LLM, and VLM will be compatible in subsequent versions

# Development Environment

- Programming Language: C/C++
- Development Platform: X5
- System Version: Ubuntu 22.04
- Compilation Toolchain: Linaro GCC 11.4.0

# Compilation

- X5 Version: Supports compilation on the X5 Ubuntu system and cross-compilation using Docker on a PC.

It also supports controlling the dependencies and functionality of the compiled pkg through compilation options.

## Dependency Libraries

- OpenCV: 3.4.5

ROS Packages:

- dnn_node
- cv_bridge
- sensor_msgs
- hbm_img_msgs
- ai_msgs
- std_srvs

hbm_img_msgs is a custom image message format used for image transmission in shared memory scenarios. The hbm_img_msgs pkg is defined in hobot_msgs; therefore, if shared memory is used for image transmission, this pkg is required.

## Compilation On Board

1. Compilation Environment Verification

- The dnn node package has been compiled.
- The hbm_img_msgs package has been compiled (see Dependency section for compilation methods).

2. Compilation

- Link third Party [llama.cpp](https://github.com/ggml-org/llama.cpp):
 
  ```shell
  git clone https://github.com/ggml-org/llama.cpp -b b4749
  cmake -B build
  cmake --build build --config Release
  # link llama.cpp to project
  cd hobot_llamacpp && ln -s thirdparty/llama.cpp llama.cpp
  ```

- Compilation command:

  ```shell
  # RDK X5
  colcon build --merge-install --cmake-args -DPLATFORM_X5=ON --packages-select hobot_llamacpp
  ```
# Notes

## Dependencies

- mipi_cam package: Publishes image messages
- usb_cam package: Publishes image messages
- websocket package: Display image messages

## Parameters

| Parameter Name      | Explanation                    | Mandatory        | Default Value  |
| ------------------- | ------------------------------ | ---------------- | -------------- |
| feed_type           | Data source, 0: vlm local; 1: vlm subscribe; 2: llm subscribe  | No                   | 2                   |
| image               | Local image path                       | No                   | config/image2.jpg     | 
| is_shared_mem_sub   | Subscribe to images using shared memory communication method | No  | 0                   |                                                                         
| llm_threads | LLM Run num of threads | No | 8 |
| model_file_name | vision model file name | No | vit_model_int16_v2.bin |
| llm_model_name | language model file name | No | qwen2.5-1.5b-instruct-q5_k_m.gguf |
| pre_infer | pre infer button | No | 0 |
| text_msg_pub_topic_name | Topic name for publishing intelligent results for tts | No                   | /tts_text |
| ros_img_sub_topic_name | Topic name for subscribing image msg | No                   | /image |
| ros_string_sub_topic_name | Topic name for subscribing string msg to set user prompt| No                   | /prompt_text |
|enable_function_call       | Whether to use the function_call feature  | No | false              |
## Instructions

- Prompts Publishing: hobot_llamacpp relies on user prompt from ros2 string msg messages. There is an example of how to use the string msg topic, where /prompt_text is the topic name. The data field contains a string that sets the prompt for the language model.

# Running

- The models required for the project need to be downloaded from the following source.

  - [Language Encoder and Decoder](https://huggingface.co/D-Robotics/InternVL2_5-1B-GGUF-BPU/blob/main/qwen2.5-1.5b-instruct-q5_k_m.gguf)

## Running on X5 Ubuntu System

Running method 1, use the executable file to start:
```shell
source ./install/setup.bash
export COLCON_CURRENT_PREFIX=./install
cp -r install/lib/hobot_llamacpp/config/ .

# Using a language model for reasoning interaction, this function is used together with the speech processing function, which will block and wait for the speech processing node to start. For details, please refer to audio_interaction.launch.py
ros2 launch qwen_llm audio_interaction.launch.py
```

# Results Analysis

## X5 results analysis
This function, when used in conjunction with the voice processing function, will block and wait for the voice processing node to start

Run：`ros2 launch qwen_llm audio_interaction.launch.py`

```bash
[INFO] [launch]: All log files can be found below /root/.ros/log/2026-01-21-15-55-24-554976-ubuntu-33128
[INFO] [launch]: Default logging verbosity is set to INFO
[INFO] [qwen_llm-1]: process started with pid [33129]
[qwen_llm-1] [WARN] [1768982125.134114779] [llamacpp_node]: This is llama cpp node!
[qwen_llm-1] [WARN] [1768982125.174208386] [llama_cpp_node]: Parameter:
[qwen_llm-1]  feed_type(0:local, 1:sub): 2
[qwen_llm-1]  image: config/image2.jpg
[qwen_llm-1]  is_shared_mem_sub: 0
[qwen_llm-1]  llm_threads: 8
[qwen_llm-1]  llm_model_name: /dev/shm/qwen2.5-1.5b-instruct-q5_k_m.gguf
[qwen_llm-1]  model_file_name: vit_model_int16_v2.bin
[qwen_llm-1]  cute_words: 你好，请问有什么能够帮助您的？
[qwen_llm-1]  user_prompt:
[qwen_llm-1]  system_prompt_file_: config/system_prompt.txt
[qwen_llm-1]  pre_infer: 0
[qwen_llm-1]  ai_msg_pub_topic_name: /llama_cpp_node
[qwen_llm-1]  text_msg_pub_topic_name: /tts_text
[qwen_llm-1]  ros_img_sub_topic_name: /image
[qwen_llm-1]  ros_string_sub_topic_name: /prompt_text
[qwen_llm-1]  enable_function_call: 0
[qwen_llm-1] [WARN] [1768982125.174306511] [llama_cpp_node]: Create ai msg publisher with topic_name: /llama_cpp_node
[qwen_llm-1] [WARN] [1768982125.197244721] [llama_cpp_node]: Create string subscription with topic_name: /prompt_text
[qwen_llm-1] build: 4749 (ee02ad02c) with cc (Ubuntu 11.2.0-19ubuntu1) 11.2.0 for aarch64-linux-gnu
[qwen_llm-1] Chat: load the model and apply lora adapter, if any
[qwen_llm-1] llama_model_loader: loaded meta data with 26 key-value pairs and 339 tensors from /dev/shm/qwen2.5-1.5b-instruct-q5_k_m.gguf (version GGUF V3 (latest))
[qwen_llm-1] llama_model_loader: Dumping metadata keys/values. Note: KV overrides do not apply in this output.
[qwen_llm-1] llama_model_loader: - kv   0:                       general.architecture str              = qwen2
[qwen_llm-1] llama_model_loader: - kv   1:                               general.type str              = model
[qwen_llm-1] llama_model_loader: - kv   2:                               general.name str              = qwen2.5-1.5b-instruct
[qwen_llm-1] llama_model_loader: - kv   3:                            general.version str              = v0.1
[qwen_llm-1] llama_model_loader: - kv   4:                           general.finetune str              = qwen2.5-1.5b-instruct
[qwen_llm-1] llama_model_loader: - kv   5:                         general.size_label str              = 1.8B
[qwen_llm-1] llama_model_loader: - kv   6:                          qwen2.block_count u32              = 28
[qwen_llm-1] llama_model_loader: - kv   7:                       qwen2.context_length u32              = 32768
[qwen_llm-1] llama_model_loader: - kv   8:                     qwen2.embedding_length u32              = 1536
[qwen_llm-1] llama_model_loader: - kv   9:                  qwen2.feed_forward_length u32              = 8960
[qwen_llm-1] llama_model_loader: - kv  10:                 qwen2.attention.head_count u32              = 12
[qwen_llm-1] llama_model_loader: - kv  11:              qwen2.attention.head_count_kv u32              = 2
[qwen_llm-1] llama_model_loader: - kv  12:                       qwen2.rope.freq_base f32              = 1000000.000000
[qwen_llm-1] llama_model_loader: - kv  13:     qwen2.attention.layer_norm_rms_epsilon f32              = 0.000001
[qwen_llm-1] llama_model_loader: - kv  14:                          general.file_type u32              = 17
[qwen_llm-1] llama_model_loader: - kv  15:                       tokenizer.ggml.model str              = gpt2
[qwen_llm-1] llama_model_loader: - kv  16:                         tokenizer.ggml.pre str              = qwen2
[qwen_llm-1] llama_model_loader: - kv  17:                      tokenizer.ggml.tokens arr[str,151936]  = ["!", "\"", "#", "$", "%", "&", "'", ...
[qwen_llm-1] llama_model_loader: - kv  18:                  tokenizer.ggml.token_type arr[i32,151936]  = [1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, ...
[qwen_llm-1] llama_model_loader: - kv  19:                      tokenizer.ggml.merges arr[str,151387]  = ["Ġ Ġ", "ĠĠ ĠĠ", "i n", "Ġ t",...
[qwen_llm-1] llama_model_loader: - kv  20:                tokenizer.ggml.eos_token_id u32              = 151645
[qwen_llm-1] llama_model_loader: - kv  21:            tokenizer.ggml.padding_token_id u32              = 151643
[qwen_llm-1] llama_model_loader: - kv  22:                tokenizer.ggml.bos_token_id u32              = 151643
[qwen_llm-1] llama_model_loader: - kv  23:               tokenizer.ggml.add_bos_token bool             = false
[qwen_llm-1] llama_model_loader: - kv  24:                    tokenizer.chat_template str              = {%- if tools %}\n    {{- '<|im_start|>...
[qwen_llm-1] llama_model_loader: - kv  25:               general.quantization_version u32              = 2
[qwen_llm-1] llama_model_loader: - type  f32:  141 tensors
[qwen_llm-1] llama_model_loader: - type q5_K:  169 tensors
[qwen_llm-1] llama_model_loader: - type q6_K:   29 tensors
[qwen_llm-1] print_info: file format = GGUF V3 (latest)
[qwen_llm-1] print_info: file type   = Q5_K - Medium
[qwen_llm-1] print_info: file size   = 1.19 GiB (5.76 BPW)
[qwen_llm-1] load: special tokens cache size = 22
[qwen_llm-1] load: token to piece cache size = 0.9310 MB
[qwen_llm-1] print_info: arch             = qwen2
[qwen_llm-1] print_info: vocab_only       = 0
[qwen_llm-1] print_info: n_ctx_train      = 32768
[qwen_llm-1] print_info: n_embd           = 1536
[qwen_llm-1] print_info: n_layer          = 28
[qwen_llm-1] print_info: n_head           = 12
[qwen_llm-1] print_info: n_head_kv        = 2
[qwen_llm-1] print_info: n_rot            = 128
[qwen_llm-1] print_info: n_swa            = 0
[qwen_llm-1] print_info: n_embd_head_k    = 128
[qwen_llm-1] print_info: n_embd_head_v    = 128
[qwen_llm-1] print_info: n_gqa            = 6
[qwen_llm-1] print_info: n_embd_k_gqa     = 256
[qwen_llm-1] print_info: n_embd_v_gqa     = 256
[qwen_llm-1] print_info: f_norm_eps       = 0.0e+00
[qwen_llm-1] print_info: f_norm_rms_eps   = 1.0e-06
[qwen_llm-1] print_info: f_clamp_kqv      = 0.0e+00
[qwen_llm-1] print_info: f_max_alibi_bias = 0.0e+00
[qwen_llm-1] print_info: f_logit_scale    = 0.0e+00
[qwen_llm-1] print_info: n_ff             = 8960
[qwen_llm-1] print_info: n_expert         = 0
[qwen_llm-1] print_info: n_expert_used    = 0
[qwen_llm-1] print_info: causal attn      = 1
[qwen_llm-1] print_info: pooling type     = 0
[qwen_llm-1] print_info: rope type        = 2
[qwen_llm-1] print_info: rope scaling     = linear
[qwen_llm-1] print_info: freq_base_train  = 1000000.0
[qwen_llm-1] print_info: freq_scale_train = 1
[qwen_llm-1] print_info: n_ctx_orig_yarn  = 32768
[qwen_llm-1] print_info: rope_finetuned   = unknown
[qwen_llm-1] print_info: ssm_d_conv       = 0
[qwen_llm-1] print_info: ssm_d_inner      = 0
[qwen_llm-1] print_info: ssm_d_state      = 0
[qwen_llm-1] print_info: ssm_dt_rank      = 0
[qwen_llm-1] print_info: ssm_dt_b_c_rms   = 0
[qwen_llm-1] print_info: model type       = 1.5B
[qwen_llm-1] print_info: model params     = 1.78 B
[qwen_llm-1] print_info: general.name     = qwen2.5-1.5b-instruct
[qwen_llm-1] print_info: vocab type       = BPE
[qwen_llm-1] print_info: n_vocab          = 151936
[qwen_llm-1] print_info: n_merges         = 151387
[qwen_llm-1] print_info: BOS token        = 151643 '<|endoftext|>'
[qwen_llm-1] print_info: EOS token        = 151645 '<|im_end|>'
[qwen_llm-1] print_info: EOT token        = 151645 '<|im_end|>'
[qwen_llm-1] print_info: PAD token        = 151643 '<|endoftext|>'
[qwen_llm-1] print_info: LF token         = 198 'Ċ'
[qwen_llm-1] print_info: FIM PRE token    = 151659 '<|fim_prefix|>'
[qwen_llm-1] print_info: FIM SUF token    = 151661 '<|fim_suffix|>'
[qwen_llm-1] print_info: FIM MID token    = 151660 '<|fim_middle|>'
[qwen_llm-1] print_info: FIM PAD token    = 151662 '<|fim_pad|>'
[qwen_llm-1] print_info: FIM REP token    = 151663 '<|repo_name|>'
[qwen_llm-1] print_info: FIM SEP token    = 151664 '<|file_sep|>'
[qwen_llm-1] print_info: EOG token        = 151643 '<|endoftext|>'
[qwen_llm-1] print_info: EOG token        = 151645 '<|im_end|>'
[qwen_llm-1] print_info: EOG token        = 151662 '<|fim_pad|>'
[qwen_llm-1] print_info: EOG token        = 151663 '<|repo_name|>'
[qwen_llm-1] print_info: EOG token        = 151664 '<|file_sep|>'
[qwen_llm-1] print_info: max token length = 256
[qwen_llm-1] load_tensors: loading model tensors, this can take a while... (mmap = true)
[qwen_llm-1] load_tensors:   CPU_Mapped model buffer size =  1220.27 MiB
```
