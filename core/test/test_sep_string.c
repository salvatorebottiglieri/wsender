#include <stdio.h>
#include <assert.h>
#include <sep_string.h>

void test_should_get_slice_return_null_when_end_index_size_is_greater_than_string_size(void){
    size_t end_index = 12;
    String* string = new_s("Hello World", 11);
    String* slice = get_slice(string, 0, end_index);
    delete_s(string);

    assert(slice == NULL);
}

