## NotEqual <a name="NotEqual"></a> {#openvino_docs_ops_comparison_NotEqual_1}

**Versioned name**: *NotEqual-1*

**Category**: Comparison binary operation

**Short description**: *NotEqual* performs element-wise comparison operation with two given tensors applying
multi-directional broadcast rules specified in the `auto_broadcast` attribute.

**Detailed description**
Before performing comparison operation, input tensors *a* and *b* are broadcasted if their shapes are different.
Broadcasting is performed according to `auto_broadcast` value.

After broadcasting, *NotEqual* does the following with the input tensors *a* and *b*:

\f[
o_{i} = a_{i} != b_{i}
\f]

**Attributes**:

* *auto_broadcast*

  * **Description**: specifies rules used for auto-broadcasting of input tensors.
  * **Range of values**:
    * *none* - no auto-broadcasting is allowed, all input shapes should match
    * *numpy* - numpy broadcasting rules, description is available in [Broadcast Rules For Elementwise Operations](../broadcast_rules.md),
    * *pdpd* - PaddlePaddle-style implicit broadcasting, description is available in [Broadcast Rules For Elementwise Operations](../broadcast_rules.md).
  * **Type**: string
  * **Default value**: "numpy"
  * **Required**: *no*

**Inputs**

* **1**: A tensor of type *T*. **Required.**
* **2**: A tensor of type *T*. **Required.**

**Outputs**

* **1**: The result of element-wise comparison operation. A tensor of type boolean.

**Types**

* *T*: arbitrary supported type.

**Examples**

*Example 1*

```xml
<layer ... type="NotEqual">
    <input>
        <port id="0">
            <dim>256</dim>
            <dim>56</dim>
        </port>
        <port id="1">
            <dim>256</dim>
            <dim>56</dim>
        </port>
    </input>
    <output>
        <port id="2">
            <dim>256</dim>
            <dim>56</dim>
        </port>
    </output>
</layer>
```

*Example 2: broadcast*
```xml
<layer ... type="NotEqual">
    <input>
        <port id="0">
            <dim>8</dim>
            <dim>1</dim>
            <dim>6</dim>
            <dim>1</dim>
        </port>
        <port id="1">
            <dim>7</dim>
            <dim>1</dim>
            <dim>5</dim>
        </port>
    </input>
    <output>
        <port id="2">
            <dim>8</dim>
            <dim>7</dim>
            <dim>6</dim>
            <dim>5</dim>
        </port>
    </output>
</layer>
```
