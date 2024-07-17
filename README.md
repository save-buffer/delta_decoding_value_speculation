# Value Speculation to accelerate Delta Decoding
This is a failed experiment. I came across this
[very cool article](https://mazzo.li/posts/value-speculation.html) about "value speculation", where you 
speculate on the value of a variable that's dependent on the previous iteration of the loop, in hopes of 
abusing the branch predictor to speculate past the branch that's introduced.

It didn't work. On my machine, normal decoding takes about 0.92 cycles/value, whiel the speculative version
takes about 1.84 cycles/row, or 2x slower. Perf seems to tell the story:

For the default implementation of decoding, perf says:
```
        22,510,310      instructions:u                   #    4.19  insn per cycle            
                                                  #    0.02  stalled cycles per insn   
         4,470,749      branches:u                       #    2.762 G/sec                     
            14,589      branch-misses:u                  #    0.33% of all branches           
```

vs the speculative implementation
```
        34,610,360      instructions:u                   #    3.54  insn per cycle            
                                                  #    0.02  stalled cycles per insn   
         8,566,764      branches:u                       #    3.370 G/sec                     
            13,752      branch-misses:u                  #    0.16% of all branches           
```

So we are executing double the number of branches, and many more instructions. 

The key difference is probably that the original blog post talked about beating the L1 cache. Here
we're trying to beat an ALU. 
