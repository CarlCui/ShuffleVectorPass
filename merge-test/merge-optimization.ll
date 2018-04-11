@str = private constant [3 x i8] c"%d " 

declare i32 @printf(i8* noalias nocapture, ...) nounwind

define <8 x i32> @merge(<4 x i32> %a, <4 x i32> %b) {
entry:
    %0 = zext <4 x i32> %a to <4 x i64>
    %1 = zext <4 x i32> %b to <4 x i64>
    %2 = shl <4 x i64> %0, <i64 32, i64 32, i64 32, i64 32> 
    %3 = or <4 x i64> %1, %2
    %4 = bitcast <4 x i64> %3 to <8 x i32>
    ret <8 x i32> %4
}

define i32 @main() {
entry:
    %p = alloca <4 x i32>

    %v1 = load <4 x i32>, <4x i32>* %p

    %v2 = insertelement <4 x i32> %v1, i32 0, i32 0 
    %v3 = insertelement <4 x i32> %v2, i32 1, i32 1 
    %v4 = insertelement <4 x i32> %v3, i32 2, i32 2 
    %v5 = insertelement <4 x i32> %v4, i32 3, i32 3 

    %result = call <8 x i32> @merge(<4 x i32> %v5, <4 x i32> %v5)

    %0 = extractelement <8 x i32> %result, i32 0
    %1 = extractelement <8 x i32> %result, i32 1
    %2 = extractelement <8 x i32> %result, i32 2
    %3 = extractelement <8 x i32> %result, i32 3
    %4 = extractelement <8 x i32> %result, i32 4
    %5 = extractelement <8 x i32> %result, i32 5
    %6 = extractelement <8 x i32> %result, i32 6
    %7 = extractelement <8 x i32> %result, i32 7

    %call0 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str , i32 0, i32 0), i32 %0)
    %call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str , i32 0, i32 0), i32 %1)
    %call2 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str , i32 0, i32 0), i32 %2)
    %call3 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str , i32 0, i32 0), i32 %3)
    %call4 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str , i32 0, i32 0), i32 %4)
    %call5 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str , i32 0, i32 0), i32 %5)
    %call6 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str , i32 0, i32 0), i32 %6)
    %call7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([3 x i8], [3 x i8]* @str , i32 0, i32 0), i32 %7)

    ret i32 0
}
