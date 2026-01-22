[English](./README.md) | 简体中文

Getting Started with qwen_llm
=======


# 功能介绍

qwen_llm package是基于 [hobot_llamacpp](https://github.com/D-Robotics/hobot_llamacpp) 适配MagicBox对话功能的大语言模型功能包

- 若想使用VLM功能，请参考hobot_llamacpp项目，该项目目前仅适配LLM，VLM将再后续版本适配

- 纯语言模型 LLM：支持系统提示词设定, 文本输入, 文本输出对话。其中文本可通过参数配置, 或运行中通过string msg 话题消息实时控制。最终输出文本, 通过 string msg 话题消息发出。

# 开发环境

- 编程语言: C/C++
- 开发平台: X5
- 系统版本：Ubuntu 22.04
- 编译工具链: Linux GCC 11.4.0

# 编译

- X5版本：支持在X5 Ubuntu系统上编译。

同时支持通过编译选项控制编译pkg的依赖和pkg的功能。

## 依赖库

- opencv:3.4.5

ros package：

- dnn node
- cv_bridge
- sensor_msgs
- ai_msgs
- hbm_img_msgs
- std_srvs

hbm_img_msgs为自定义的图片消息格式, 用于shared mem场景下的图片传输, hbm_img_msgs pkg定义在hobot_msgs中, 因此如果使用shared mem进行图片传输, 需要依赖此pkg。

## 编译选项

1、SHARED_MEM

- shared mem（共享内存传输）使能开关, 默认打开（ON）, 编译时使用-DSHARED_MEM=OFF命令关闭。
- 如果打开, 编译和运行会依赖hbm_img_msgs pkg, 并且需要使用tros进行编译。
- 如果关闭, 编译和运行不依赖hbm_img_msgs pkg, 支持使用原生ros和tros进行编译。
- 对于shared mem通信方式, 当前只支持订阅nv12格式图片。

## 板端 Ubuntu系统上编译

1、编译环境确认

- 板端已安装X5 Ubuntu系统。
- 当前编译终端已设置TogetherROS环境变量：`source PATH/setup.bash`。其中PATH为TogetherROS的安装路径。
- 已安装ROS2编译工具colcon。安装的ROS不包含编译工具colcon, 需要手动安装colcon。colcon安装命令：`pip install -U colcon-common-extensions`

2、编译依赖

- 链接第三方仓库 [llama.cpp](https://github.com/ggml-org/llama.cpp):
 
```shell
git clone https://github.com/ggml-org/llama.cpp -b b4749
cmake -B build
cmake --build build --config Release
# 链接llama.cpp到工程目录下
cd qwen_llm && ln -s thirdparty/llama.cpp llama.cpp
```

3、编译

- 编译命令：

```shell
# RDK X5
colcon build --cmake-args -DPLATFORM_X5=ON --packages-select qwen_llm
```


# 使用介绍


## 参数

| 参数名                    | 解释                                  | 是否必须             | 默认值              |
| ------------------ | ------------------------------------- | -------------------- | ------------------- |
| feed_type                 | 数据来源, 0：VLM 本地；1：VLM 订阅; 2: LLM 订阅      | 否 | 2                                 |
| image                     | 本地图片地址                                        | 否 | config/image2.jpg                 |
| is_shared_mem_sub         | 使用shared mem通信方式订阅图片                      | 否  | 0                                 |
| llm_threads               | 语言模型推理线程数                                  | 否 | 8                                  |
| model_file_name           | 视觉模型模型名                                      | 否 | "vit_model_int16_v2.bin""          |
| llm_model_name            | 语言模型模型名                                      | 否 | "qwen2.5-1.5b-instruct-q5_k_m.gguf"|
| pre_infer                 | 提前推理开关                                        | 否 | 0                                  |
| text_msg_pub_topic_name   | 发布智能结果的topicname,中间结果                     | 否 | /tts_text                          |
| ros_img_sub_topic_name    | 接收ros图片话题名                                   | 否 | /image                             |
| ros_string_sub_topic_name | 接收string消息话题获得文本提示词                     | 否 | /prompt_text                       |
| enable_function_call      | 是否使用function_call功能                           | 否 | false                              |


## 使用说明

- 发布提示词：qwen_llm 依赖string msg话题消息获取提示词。string msg话题使用示例如下。其中 /prompt_text 为话题名。data字段中的数据为string字符串, 设置语言模型提示词
- 该功能包支持实现类似function_call的功能，但由于涉及大量prompt的输入，模型初始化时间较长，所以默认不开启，若想体验，可参考audio_interaction.launch.py，同步启动舵机控制节点
- llama_warmup主要用于实现模型预热，启动正式功能前运行，能够保证推理速度稳定

## 运行

- qwen_llm 模型链接。

  - [语言编解码模型](https://huggingface.co/D-Robotics/InternVL2_5-1B-GGUF-BPU/blob/main/qwen2.5-1.5b-instruct-q5_k_m.gguf)

## X5 Ubuntu系统上运行

运行方式1, 使用可执行文件启动：
```shell
export COLCON_CURRENT_PREFIX=./install
source ./install/local_setup.bash
# config中为示例使用的模型, 回灌使用的本地图片
# 根据实际安装路径进行拷贝（docker中的安装路径为install/lib/qwen_llm/config/, 拷贝命令为cp -r install/lib/qwen_llm/config/ .）。
cp -r install/lib/qwen_llm/config/ .

# 使用语言模型进行推理交互，该功能与语音处理功能一起使用，将阻塞等待语音处理节点启动，具体参考audio_interaction.launch.py
ros2 launch qwen_llm audio_interaction.launch.py
```

# 结果分析

## X5结果展示

### 语言模型
该功能与语音处理功能一起使用，将阻塞等待语音处理节点启动

运行命令：`ros2 launch qwen_llm audio_interaction.launch.py`

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
