; ModuleID = '<stdin>'
source_filename = "TestFiles/signed_orequal/signed_orequal.c"
target datalayout = "e-m:o-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-apple-macosx10.14.0"

; Function Attrs: noinline nounwind ssp uwtable
define i32 @main() #0 {
entry:
  br label %while.cond

while.cond:                                       ; preds = %while.body, %entry
  %i.0 = phi i32 [ 0, %entry ], [ %inc, %while.body ]
  %cmp = icmp slt i32 %i.0, 100
  br i1 %cmp, label %while.body, label %while.cond1

while.body:                                       ; preds = %while.cond
  %vSSA_sigma = phi i32 [ %i.0, %while.cond ]
  %inc = add nsw i32 %vSSA_sigma, 1
  %call = call i32 (...) @notrem()
  br label %while.cond

while.cond1:                                      ; preds = %while.cond, %while.body3
  %j.0 = phi i32 [ %inc4, %while.body3 ], [ 0, %while.cond ]
  %cmp2 = icmp slt i32 %j.0, 100
  br i1 %cmp2, label %while.body3, label %while.cond7

while.body3:                                      ; preds = %while.cond1
  %vSSA_sigma2 = phi i32 [ %j.0, %while.cond1 ]
  %inc4 = add nsw i32 %vSSA_sigma2, 1
  %call5 = call i32 (...) @notrem()
  br label %while.cond1

while.cond7:                                      ; preds = %while.cond1, %while.body9
  %k.0 = phi i32 [ %inc10, %while.body9 ], [ 0, %while.cond1 ]
  %cmp8 = icmp slt i32 %k.0, 100
  br i1 %cmp8, label %while.body9, label %while.cond13

while.body9:                                      ; preds = %while.cond7
  %vSSA_sigma4 = phi i32 [ %k.0, %while.cond7 ]
  %inc10 = add nsw i32 %vSSA_sigma4, 1
  %call11 = call i32 (...) @notrem()
  br label %while.cond7

while.cond13:                                     ; preds = %while.cond7, %while.body15
  %l.0 = phi i32 [ %inc16, %while.body15 ], [ 0, %while.cond7 ]
  %cmp14 = icmp slt i32 %l.0, 100
  br i1 %cmp14, label %while.body15, label %while.cond19

while.body15:                                     ; preds = %while.cond13
  %vSSA_sigma6 = phi i32 [ %l.0, %while.cond13 ]
  %inc16 = add nsw i32 %vSSA_sigma6, 1
  %call17 = call i32 (...) @notrem()
  br label %while.cond13

while.cond19:                                     ; preds = %while.cond13, %while.body21
  %c.0 = phi i32 [ %inc22, %while.body21 ], [ 101, %while.cond13 ]
  %cmp20 = icmp slt i32 %c.0, 200
  br i1 %cmp20, label %while.body21, label %if.then

while.body21:                                     ; preds = %while.cond19
  %vSSA_sigma7 = phi i32 [ %c.0, %while.cond19 ]
  %inc22 = add nsw i32 %vSSA_sigma7, 1
  %call23 = call i32 (...) @notrem()
  br label %while.cond19

if.then:                                          ; preds = %while.cond19
  %call26 = call i32 (...) @notrem()
  %call29 = call i32 (...) @notrem()
  %call38 = call i32 (...) @notrem()
  %call43 = call i32 (...) @notrem()
  ret i32 0
}

declare i32 @notrem(...) #1

declare i32 @rem(...) #1

attributes #0 = { noinline nounwind ssp uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "min-legal-vector-width"="0" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="penryn" "target-features"="+cx16,+fxsr,+mmx,+sahf,+sse,+sse2,+sse3,+sse4.1,+ssse3,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }

!llvm.module.flags = !{!0, !1}
!llvm.ident = !{!2}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{i32 7, !"PIC Level", i32 2}
!2 = !{!"clang version 8.0.0 (tags/RELEASE_800/final 360744)"}
