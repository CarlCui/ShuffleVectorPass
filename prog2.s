 define i32 @main() #0 {
    %1 = alloca <4 x i32>
    %2 = load <4 x i32>, <4x i32>* %1
    %3 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 0, i32 1, i32 2, i32 3>

    ret i32 0
}