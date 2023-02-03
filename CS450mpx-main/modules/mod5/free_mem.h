/*
Procedure..: free memory
Description..: Will free the allocated memory and return a 0 for fail and 1 for success.
Will also combine conseutive blocks of free memory.
Params..: pointer to the memory block to be freed.
*/
int free_mem(void* block);