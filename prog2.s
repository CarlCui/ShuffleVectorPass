 define i32 @main() #0 {
    %1 = alloca <4 x i32>
    %2 = load <4 x i32>, <4x i32>* %1
    %a161 = alloca <16 x i32>
    %a162 = load <16 x i32>, <16 x i32>* %a161
    %3 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 0, i32 1, i32 2, i32 3>
    %4 = shufflevector <16 x i32> %a162, <16 x i32> undef,
                        <16 x i32> <i32 1, i32 2, i32 3, i32 4,
                                    i32 5, i32 6, i32 7, i32 8,
                                    i32 9, i32 10, i32 11, i32 12,
                                    i32 13, i32 14, i32 15, i32 0> ;; rotate left by 1
    %5 = shufflevector <16 x i32> %a162, <16 x i32> undef,
                        <16 x i32> <i32 1, i32 1, i32 1, i32 1,
                                    i32 1, i32 1, i32 1, i32 1,
                                    i32 1, i32 1, i32 1, i32 1,
                                    i32 1, i32 1, i32 1, i32 1> ;; rotate left by 1
    %aaa = load <4 x i32>, <4 x i32>* %1
    %6 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 2, i32 3, i32 0, i32 1> ;; rotate left by 2
    %7 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 2, i32 3, i32 0, i32 1> ;; rotate left by 2
    %8 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 2, i32 3, i32 0, i32 1> ;; rotate left by 2
    %9 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 2, i32 3, i32 0, i32 1> ;; rotate left by 2
    %10 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 2, i32 3, i32 0, i32 1> ;; rotate left by 2
    %11 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 2, i32 3, i32 0, i32 1> ;; rotate left by 2
    %12 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 1, i32 2, i32 3, i32 0> ;; rotate left by 2
    %13 = shufflevector <4 x i32> %2, <4 x i32> undef,
                        <4 x i32> <i32 3, i32 0, i32 1, i32 2> ;; rotate left by 2
    %14 = shufflevector <4 x i32> %2, <4 x i32> %2,
                        <4 x i32> <i32 0, i32 5, i32 2, i32 3> ;; merge
    %15 = shufflevector <4 x i32> %2, <4 x i32> %2,
                        <4 x i32> <i32 0, i32 5, i32 6, i32 7> ;; merge 2
    %16 = shufflevector <4 x i32> %2, <4 x i32> %2,
                        <4 x i32> <i32 0, i32 2, i32 6, i32 7> ;; false merge

    ret i32 0
}