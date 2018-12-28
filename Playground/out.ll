; ModuleID = 'top'
source_filename = "top"

@0 = private unnamed_addr constant [13 x i8] c"hello world!\00"
@1 = private unnamed_addr constant [4 x i8] c"%d\0A\00"
@2 = private unnamed_addr constant [12 x i8] c"you r right\00"
@3 = private unnamed_addr constant [12 x i8] c"you r wrong\00"

define i32 @main() {
entrypoint:
  %0 = alloca i32
  store i32 1, i32* %0
  %1 = load i32, i32* %0
  %2 = trunc i32 %1 to i1
  %3 = icmp ne i1 %2, false
  br i1 %3, label %"when true", label %"when false"

"when true":                                      ; preds = %entrypoint
  %4 = call i32 @puts(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @2, i32 0, i32 0))
  br label %"after if"

"when false":                                     ; preds = %entrypoint
  %5 = call i32 @puts(i8* getelementptr inbounds ([12 x i8], [12 x i8]* @3, i32 0, i32 0))
  br label %"after if"

"after if":                                       ; preds = %"when false", %"when true"
  %6 = load i32, i32* %0
  %7 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([4 x i8], [4 x i8]* @1, i32 0, i32 0), i32 %6)
  ret i32 0
}

declare i32 @puts(i8*)

declare i32 @printf(i8*, ...)
