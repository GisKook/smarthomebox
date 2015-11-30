
struct sqlitedb;
struct sqlitedb * sqlitedb_create(char * filepath);
void sqlitedb_exec(struct sqlitedb *, char *sql);
void sqlitedb_close_table(struct sqlitedb * sdb);
void sqlitedb_insert_record(struct sqlitedb *,char * table,int DID,int  CID,char * name,char *end);
char **sqlitedb_search_all(struct sqlitedb *,char * table);
char **sqlitedb_search_id(struct sqlitedb *,char * table,int id);
void sqlitedb_delete_id(struct sqlitedb *,char * table,int id);
void sqlitedb_delete_all(struct sqlitedb *,char * table);
//void updata_data(sqlite3 * db);
void sqlitedb_updata_data(struct sqlitedb *,int terminalID,char *terminalname);
