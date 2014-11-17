# Benchmarking MRI’s hash implementation

The point of this experiment is to benchmark MRI's hash table implementation vs others to see if it can be improved upon.

Right now there are two contenders:

- st (which is bundled with Ruby itself, and slightly modified)
- uthash (can be found [here](https://github.com/troydhanson/uthash))

## Results

__This is very incomplete, more to come.__

### 1,000,000 new writes:

```
st time (wall):     1.06
uthash time (wall): 0.38
```
