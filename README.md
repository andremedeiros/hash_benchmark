# Benchmarking MRI’s hash implementation

The point of this experiment is to benchmark MRI's hash table implementation vs others to see if it can be improved upon.

Right now there are two contenders:

- st (not the modified one bundled with Ruby, as I wasn't able to get it to work yet.)
- uthash (can be found [here](https://github.com/troydhanson/uthash))

Code is compiled with `-O3` and binds to `jemalloc`

## Results

__This is very incomplete, more to come.__

All these tests are run with:
- 2 runs
- 100,000 iterations each
- 100 different keys on each iteration

### Writes

```
st:     64079.36 i/sec  (3.121130 seconds)
uthash: 128423.94 i/sec (1.557342 seconds)
```

### Reads

```
st:     362623.60 i/sec (0.551536 seconds)
uthash: 275530.29 i/sec (0.725873 seconds)
```

### Non-existant key reads

```
st:    391402.46 i/sec (0.510983 seconds)
uthash 352387.45 i/sec (0.567557 seconds)
```
