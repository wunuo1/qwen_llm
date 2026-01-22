# Copyright (c) 2024，D-Robotics.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import os

from launch import LaunchDescription
from launch_ros.actions import Node

from launch.actions import IncludeLaunchDescription, SetEnvironmentVariable, DeclareLaunchArgument
from launch.launch_description_sources import PythonLaunchDescriptionSource
from ament_index_python import get_package_share_directory
from launch.substitutions import TextSubstitution, LaunchConfiguration
from launch.conditions import IfCondition

def generate_launch_description():

    enable_function_call_arg = DeclareLaunchArgument(
        "enable_function_call", default_value="False"
    )

    audio_node = Node(
        package='audio_io',
        executable='audio_io',
        output='screen',
        parameters=[
            {"tts_config_path": "/userdata/MagicBox/dep/matcha-icefall-zh-baker",
            "asr_model_path": "/userdata/MagicBox/config/"},
        ],
        arguments=['--ros-args', '--log-level', 'warn']
    )

    qwen_llm_node = Node(
        package='qwen_llm',
        executable='qwen_llm',
        output='screen',
        parameters=[
            {"feed_type": 2},
            {"llm_model_name": "/dev/shm/qwen2.5-1.5b-instruct-q5_k_m.gguf"},
            {"system_prompt_file_": "config/system_prompt.txt"},
            {"cute_words": "你好，请问有什么能够帮助您的？"},
            {"system_prompt_function_call_file": "config/system_prompt_function_call.txt"},
            {"enable_function_call": LaunchConfiguration('enable_function_call')},
        ],
        arguments=['--ros-args', '--log-level', 'warn']
    )

    fc_call_node = Node(
        package='gesture_legs_control',
        executable='function_call_control',
        output='screen',
        arguments=['--ros-args', '--log-level', 'info'],
        condition=IfCondition(LaunchConfiguration('enable_function_call')),
    )

    return LaunchDescription([
        SetEnvironmentVariable(
            'RMW_IMPLEMENTATION', 'rmw_cyclonedds_cpp'
        ),
        enable_function_call_arg,
        # audio_node,
        fc_call_node,
        qwen_llm_node
    ])