# Tiny KV

## 题目描述

某个机器的配置为：CPU 8 cores，MEM 4G，HDD 4T。

这个机器上有一个 1T 的无序数据文件，格式为 `key_size, key, value_size, value`，所有 key 都不相同。

- key 的长度符合 [1B, 1KB] 的 uniform 分布。
- value 的长度符合 [1B, 1MB] 的 uniform 分布。

设计一个索引结构，使得并发随机地读取每一个 key-value 的代价最小；读取时 key 必然在文件中且大致符合齐夫分布。

允许对数据文件做任意预处理，但是预处理的时间计入到整个读取过程的代价里。

