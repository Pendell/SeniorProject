@str = private unnamed_addr constant [15 x i8] c"Hello, world!\0A\00"

; Function Attrs: noinline nounwind optnone
define dso_local i32 @main() #0 {
  %1 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([15 x i8], [15 x i8]* @.str, i32 0, i32 0))
  ret i32 0
}