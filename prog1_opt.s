; ModuleID = 'prog1.s'
source_filename = "prog1.s"

define i32 @main() {
  %1 = alloca i32, align 4
  store i32 0, i32* %1
  ret i32 0
}
