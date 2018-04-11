These are files for testing merge pattern optimizer, together with assembly

merge.ll is IR code file, and merge.s contains assembly

merge-unalign.ll using <5 x i5> instead of regular <4 x i32>

Running the following command under root directory to test optimizer:

opt -S -load build/ShuffleVectorPass/ShuffleVectorPass.so -sv merge-test/merge.ll 

And the output should be the same as in merge-optimization.ll
