#!/bin/bash

# 输出文件
OUTPUT_FILE="results.csv"

# 初始化输出文件
echo "Test,Latency (ns),Min Granularity (ns),Wakeup Granularity (ns),Execution Time (s)" > $OUTPUT_FILE

# 测试矩阵乘法程序
test_matrix_mult() {
  local latency=$1
  local min_gran=$2
  local wakeup_gran=$3
  local scheduler=$4

  # 设置调度器参数
  echo $latency > /proc/sys/kernel/sched_latency_ns
  echo $min_gran > /proc/sys/kernel/sched_min_granularity_ns
  echo $wakeup_gran > /proc/sys/kernel/sched_wakeup_granularity_ns

  # 记录开始时间
  local start_time=$(date +%s%N)

  # 执行测试程序
  if [ "$scheduler" == "realtime" ]; then
    chrt -f 99 ./matrix_mult
  else
    ./matrix_mult
  fi

  # 记录结束时间
  local end_time=$(date +%s%N)
  local execution_time=$(echo "scale=6; ($end_time - $start_time) / 1000000000" | bc)

  # 保存结果到输出文件
  echo "$scheduler,$latency,$min_gran,$wakeup_gran,$execution_time" >> $OUTPUT_FILE
}

# 不同调度器参数组合
declare -a latencies=("1000000" "2000000" "5000000")
declare -a min_grans=("100000" "200000" "500000")
declare -a wakeup_grans=("100000" "200000" "500000")

# 运行普通调度器测试
for latency in "${latencies[@]}"; do
  for min_gran in "${min_grans[@]}"; do
    for wakeup_gran in "${wakeup_grans[@]}"; do
      test_matrix_mult $latency $min_gran $wakeup_gran "normal"
    done
  done
done

# 运行实时调度器测试
for latency in "${latencies[@]}"; do
  for min_gran in "${min_grans[@]}"; do
    for wakeup_gran in "${wakeup_grans[@]}"; do
      test_matrix_mult $latency $min_gran $wakeup_gran "realtime"
    done
  done
done

echo "Tests completed. Results saved to $OUTPUT_FILE."

