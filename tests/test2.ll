

; ModuleID = 'test1-m2r.bc'
source_filename = "test1.c"
target datalayout = "e-m:e-p:32:32-p270:32:32-p271:32:32-p272:64:64-f64:32:64-f80:32-n8:16:32-S128"
target triple = "i686-pc-linux-gnu"
; Function Attrs: noinline nounwind
define dso_local i32 @foo(i32 %0, i32 %1, i32 %2) #0 {
  %4 = icmp sgt i32 %0, 0
  br i1 %4, label %5, label %6

5:                                                ; preds = %3
  br label %7

6:                                                ; preds = %3
  br label %7

7:                                                ; preds = %6, %5
  %.01 = phi i32 [ %1, %5 ], [ 0, %6 ]
  %8 = add nsw i32 %.01, 2
  %9 = icmp sgt i32 %1, 0
  br i1 %9, label %10, label %11

10:                                               ; preds = %7
  br label %12

11:                                               ; preds = %7
  br label %12

12:                                               ; preds = %11, %10
  %.0 = phi i32 [ %8, %10 ], [ 0, %11 ]
  %13 = add nsw i32 %8, %.0
  ret i32 %13
}

attributes #0 = { noinline nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "frame-pointer"="all" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="pentium4" "target-features"="+cx8,+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"NumRegisterParameters", i32 0}
!1 = !{i32 1, !"wchar_size", i32 4}
!2 = !{!"clang version 10.0.0 (https://github.com/llvm/llvm-project.git 1cc8fff420a76ae869f73ce2b19e7c1fc73da3ed)"}
