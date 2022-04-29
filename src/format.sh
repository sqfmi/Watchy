#!/ bin / bash
STYLE_OPT = "{BasedOnStyle: llvm, AlignConsecutiveMacros: true, "
            "AlignConsecutiveAssignments: true}" clang -
            format-- style = "$STYLE_OPT" - i *
