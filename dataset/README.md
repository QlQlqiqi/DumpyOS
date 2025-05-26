config.ini 是控制的配置文件，expr sec 中 dataset 指定的是用什么数据集，目前有 4 种。需要配置 rootfn 作为数据集的根目录，还需要配置 time series 数据点个数。比如对于数据集 gas 来说，dataset 中会有一个目录 gas，然后在其下面会有各个目录，其中 data 目录是保存原数据 time series 的，这个数据来自于下面的**注释 1**。（如果没有配置 rootfn，那么就需要配置各种 fn，详见 Const.h）

首先是 build Fadas fuzzy index，那么设置 config.ini 中 index = 1、ops = 0，materialized = 1。这时候可能会出现以下几种错误：

- 如果是长时间卡死，那么大概率是每个页大小比较小，需要分裂较多次数，可以调高 config.ini 中 th 这个参数，这个不同数据集和数据规模对这个参数的要求不同，可以看下面的**注释 2**；
- 还有可能报其他异常，那么可能是目录没设置正确，或者目录不存在，并且可以尝试在数据集的目录（比如 ~/DumpyOS/dataset/gas 下）运行 ../rm_bin.sh，它会移除除了 data 以外的所有文件，然后重试。

其次是 approx search in Fadas fuzzy index，那么设置 config.ini 中 index = 1、ops = 1，materialized = 1。search 过程中可能会边 search 边从 disk 中读 query，可以设置 config.ini 中 read_file_while_search = 0 来提前读取 querys，并且不算在 qps 的耗时中。config.ini 的 query_num 是测试的 query 数量，maxK 就是 topk 的意思。 

也可以在运行 FADAS 的时候，增加 build 或者 search 参数，这样可以不用配置 ops，如：./FADAS build 和 ./FADAS search。

目前所有的数据集点都是归一化的

目前 DumpyOS 尚存一个问题（是否是问题有待考证）：当 node 中数据量超过 Const::th 时，需要进行 split，但是代码中每次 split 前后数据量减少的幅度极低，比如几百 ms 时间进行一次 split，但是只减少几十数据量，效率过低。

如果 build 时候一直输出某个数字，那么是在进行 split，直到其小于 Const::th，可以通过调大这个参数解决。




##### 注释 1

在 StsCache 的 fasDTW 分支，配置 config.ini 中 common sec 的 dataset 为想要的数据集，一共 4 个选择，然后设置 forest sec 中 ts_dim 指定 time series 的数据点个数，在 hnsw_SAX_test.cpp 中修改 MAX_TS_NUM 来指定需要生成的 ts 数量，并在 int main 中关闭 Test、调用 GenerateDataFile 来生成文件，文件名可能为 gas-16-100k_16.bin，代表 gas 数据集、ts 有 16 个数据点、100k 个 ts，如果 ts 数量是 1001，那么也是 1k，所以建议数量为 1k 的整数倍，不然会产生文件重叠。如果是 stock 数据集，hnsw_SAX_test.cpp 中 TAG_IDX_END 参数建议修改为 1000，其他的数据集 300 即可。生成的文件在 test/dataset 中，然后将其 copy 到 DumpyOS 指定位置即可。这些生成的数据集都是归一化后的，范围是 [0, 1)。



##### 注释 2

这个只是已经测试成功的：（文件名意义见注释 1）

xxx-16-200k_16.bin 可以 200k，低于 100k 不行。（因为数据集是归一化后的，所以 xxx 代表所有数据集）