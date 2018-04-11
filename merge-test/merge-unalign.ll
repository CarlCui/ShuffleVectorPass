@str = private constant [3 x i8] c"%d " 

declare i32 @printf(i8* noalias nocapture, ...) nounwind

define <10 x i5> @merge(<5 x i5> %a, <5 x i5> %b) {
entry:
  %t = shufflevector <5 x i5> %a, <5 x i5> %b, <10 x i32> <i32 0, i32 5, i32 1, i32 6, i32 2, i32 7, i32 3, i32 8, i32 4, i32 9>
  ret <10 x i5> %t
}

define i32 @main() {
entry:
    %p = alloca <5 x i5>

    %v1 = load <5 x i5>, <5 x i5>* %p

    %v2 = insertelement <5 x i5> %v1, i5 0, i5 0 
    %v3 = insertelement <5 x i5> %v2, i5 1, i5 1 
    %v4 = insertelement <5 x i5> %v3, i5 2, i5 2 
    %v5 = insertelement <5 x i5> %v4, i5 3, i5 3 
    %v6 = insertelement <5 x i5> %v5, i5 4, i5 4 

    %result1 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)
    %result2 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)
    %result3 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)
    %result4 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)
    %result5 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)
    %result6 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)
    %result7 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)
    %result8 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)
    %result9 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)
    %result0 = call <10 x i5> @merge(<5 x i5> %v6, <5 x i5> %v6)

    ret i32 0
}
