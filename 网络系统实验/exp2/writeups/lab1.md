# Lab 1 Writeup
徐亦昶  
PB20000156
## push_substring流程
首先判断数据包大小是否超过缓存，如果超过就将其丢弃：
```c++
if (index >= _head_index + _capacity || index + data.length() > _head_index + _capacity){
    return;
}
```
接下来考虑起始位置在_head_index之前的情况，分两种情况讨论：
* 如果数据包末尾超过了_head_index，则将超过的部分追加到缓存中并更新_head_index。
* 否则说明整个数据包已经在之前被读入缓存，将其丢弃。

否则将_unassembled_byte加上数据包的大小。扫描待重组的数据块集合_blocks，先找到起始位置不小于index的数据块，将其和index合并，并将_unassembled_byte减去因合并而被消除的重叠数据块大小。

最后，扫描一遍_blocks，不断将其实地址等于_head_index的数据块合并到已重组的数据流中并更新_head_index，直到不能合并。

上述操作完成后，检验eof参数的值，如果为true，将类的_eof标签置为true。进一步，如果_eof被置为true且缓存中的数据库已完成重组（当前缓存集合为空），则设置已重组数据流的eof为1（调用end_input)，
## 数据块合并是如何判断两个数据是否具备合并的条件的
首先将两个数据块按照起始位置排序，小的排前面。排好序后，根据第一个数据块起始位置和长度即可确定两个数据块是否重叠，如果重叠就可合并。这里可以把两个数据块并排相邻放置的情况当成“退化的重叠”。当然，为了对其进行进一步处理，还要对重叠的情况作进一步分类，如是交错关系还是包含关系。
## 待重组数据长度以及已重组数据长度如何计算
* 待重组数据长度：使用_unassembled_byte在数据流读入重组过程中实时维护。读入数据时将其增大，重组过程删除重叠部分或将数据块放入已重组区域时减小。
* 已重组数据长度：直接返回_output的大小。
## 难点和解决思路
### 难点1：数据的合并操作
难点在于合并条件比较复杂，需要分类讨论。在初次做这部分实验时，没用考虑两个数据块相互包含的情形，导致在先输入abc再输入b这种情况下会出错（计算出的重叠数据块大小会出错）。这个问题是通过在数据块合并函数调用位置后输出当前缓存区和_unassembled_byte定位的，经过对merge_block的重新审查修改了这个错误。
### 难点2：部分代码的理解
第二个难点在于这部分代码的理解：
```c++
auto iter = _blocks.lower_bound(elm);
while(iter != _blocks.end() && (merged_bytes = merge_block(elm, *iter)) >= 0)
{
    _unassembled_byte -= merged_bytes;
    _blocks.erase(iter);
    iter = _blocks.lower_bound(elm);
}
```
首先是lower_bound，它得到的是**不小于**elm的第一个元素，我一开始理解成了字面意义上的不大于elm的第一个元素，并把另一个与之类似的函数upper_bound理解成了不小于elm的第一个元素。在调试过程中发现lower_bound返回的总是和预期的不一致，即使输出中间结果也找不到原因。经过反复查阅资料，才知道是把这个函数的目的理解错了。正确的答案是：lower_bound得到第一个不小于elm的元素，upper_bound得到第一个严格大于elm的元素。想找到第一个小于elm的元素，需要先求lower_bound再回退一个元素的位置。

其次是对merge_block返回值的理解。我一开始理解成了合并后数据块的大小，发现这样在某些情形下_unassembled_byte会被减为负值。后来经过思考，得知它返回的应该是重叠数据块的大小。
## 本实验说明的改进之处
我认为应该在文档中说明一下merge_block返回值的意义