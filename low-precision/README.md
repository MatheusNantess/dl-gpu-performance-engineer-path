# Low Precision

Reduced precision formats: what they cost in accuracy, and what they buy in speed.

```text
fp16/           # half precision, loss scaling, overflow in the backward
bf16/           # FP32 range, fewer mantissa bits
fp8/            # E4M3/E5M2 -- cloud hardware only
quantization/   # PTQ, QAT, INT8, weight-only
```

## Hardware reality

| Format | RTX 3060 (Ampere GA106) |
| --- | --- |
| FP32 | Yes |
| TF32 | Yes -- automatic for some matmul paths; a fair benchmark states whether it is enabled |
| FP16 | Yes, with Tensor Core support |
| BF16 | Yes |
| **FP8** | **No.** Ada/Hopper only |

`fp8/` stays empty until cloud hardware is available. That is a stated limitation, not a gap to quietly fill with theory.

## What matters for kernel work

- **Speed without accuracy is not a result.** Every low-precision benchmark is paired with an error measurement against an FP32 reference.
- **The backward is where precision fails first.** Gradients have wider dynamic range than activations; FP16 overflow and underflow show up there before anywhere else.
- **Accumulation precision is a separate decision from storage precision.** Computing in FP16 while accumulating in FP32 is standard for a reason.
- **Tensor Cores have layout and shape requirements.** Miss them and the hardware silently falls back to the slower path -- which is why measured utilization matters more than the assumption that the dtype was enough.