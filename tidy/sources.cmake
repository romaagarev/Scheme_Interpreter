add_library(scheme_tidy 
    tokenizer.cpp
    parser.cpp
    scheme.cpp
        boolean_running.cpp
        conditional.cpp
        eval_running.cpp
        integer_running.cpp
        list_running.cpp
        object.cpp
        running.cpp
        tokentypechecker.cpp
        variables.cpp
        heap.cpp
        # maybe more .cpp files here
)
