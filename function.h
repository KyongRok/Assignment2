/**
 * Full Name: KyongRok Kim 215813413
 * CourseID: EECS3221
 * Discription: holds all the function headers.
 */
void banker_algo(struct process* p , struct resource_manager res_man, struct instruction* inst);
void initiate(struct process** p1 , struct resource_manager* res_man1 , int process_id , int resource_type , int resource_amount);
int request(struct process** p1 , struct resource_manager* res_man1, int process_id, int resouce_type , int resouce_amount);
int isSafe(struct process p1 , struct resource_manager res_man1, int resouce_type , int resouce_amount);
void release(struct collector* col , struct process** p1, int process_id , int resource_type, int resouce_amount);
int sort_by_priority(const void* a, const void* b);

