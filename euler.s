@.str = private constant [23 x i8] c"GCD of %d and %d = %d\0A\00", align 1
@.num = private constant [4 x i8] c"%d\0A\00", align 1

declare i32 @printf(i8* noalias nocapture, ...) nounwind

define i32 @euler(i32 %m, i32 %n) nounwind uwtable {
    %p = srem i32 %m, %n

    %cast = getelementptr inbounds [4 x i8], [4 x i8]* @.num, i32 0, i32 0

    call i32 (i8*, ...) @printf(i8* %cast, i32 %m)
    call i32 (i8*, ...) @printf(i8* %cast, i32 %n)
    call i32 (i8*, ...) @printf(i8* %cast, i32 %p)

    %cond = icmp eq i32 %p, 0
    br i1 %cond, label %cond.true, label %cond.false

cond.true:
    ret i32 %n
cond.false:
    %call = call i32 @euler(i32 %n, i32 %p)
    ret i32 %call
}

define i32 @main() {
    %call = call i32 @euler(i32 25, i32 15)

    %call1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([23 x i8], [23 x i8]* @.str, i32 0, i32 0), i32 25, i32 15, i32 %call)

    ret i32 0
}