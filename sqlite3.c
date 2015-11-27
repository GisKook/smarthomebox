#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sqlite3.h"
//static int sn = 0;

struct sqlitedb{ 
	sqlite3 * db;
};

struct sqlitedb * sqlitedb_create(char * filepath){
	struct sqlitedb * sdb = (struct sqlitedb *) malloc(sizeof(struct sqlitedb));
	memset(sdb,0,sizeof(struct sqlitedb));

	char * zErrMsg = 0;
	int rc;

	rc = sqlite3_open(filepath,&sdb->db);
	if (rc) {
		fprintf(stderr,"can't open database%s\n",sqlite3_errmsg(sdb->db));
		sqlite3_close(sdb->db);
		free(sdb);
		return NULL;
	}

	return sdb;
}

void sqlitedb_exec(struct sqlitedb * sdb, char *sql){
	char * zErrMsg;
	sqlite3_exec(sdb->db,sql,0,0,&zErrMsg);
}


void sqlitedb_close_table(struct sqlitedb * sdb)
{
	sqlite3_close(sdb->db);
}


void sqlitedb_insert_record(struct sqlitedb *sdb,char * table,int DID,int  CID,char * name,char *end)
{
	char * sql;
	char * zErrMsg = NULL;

	sql = sqlite3_mprintf("insert into %s values(%d,'%d','%s','%s')",table,DID,CID,name,end);
	sqlite3_exec(sdb->db,sql,0,0,&zErrMsg);
	sqlite3_free(sql);
}


char **sqlitedb_search_all(struct sqlitedb *sdb,char * table)
{
	char * sql;
	char * zErrMsg = 0;
	int i,offset = 0;
	char * buf,* tmp;
	char zSql[256];
	char **datatable; /* the result table*/
	int nRow; /* the number of rows in the result */
	int nColumn; /* the number of columns of result */
	int rc;
	datatable = (char**)malloc(100*sizeof(char*));

	sqlite3_snprintf(sizeof(zSql), zSql,
			"select * from %s", table);

	rc=sqlite3_get_table(sdb->db, zSql,

			&datatable, &nRow, &nColumn, &zErrMsg);

	if( rc!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
		exit(1);
	} 


	return datatable;

}    

char **sqlitedb_search_id(struct sqlitedb *sdb, char * table,int id)
{
	char zSql[256];
	char *szError = NULL,**dbResult;
	int row,col;
	dbResult = (char**)malloc(100*sizeof(char*));
	sqlite3_snprintf(sizeof(zSql), zSql,
			"select * from %s where DeviceID=%d",table,id);


	int result = sqlite3_get_table( sdb->db,zSql,&dbResult,&row,&col,&szError );
	if( result!=SQLITE_OK )
	{
		fprintf(stderr, "SQL error: %s\n", szError);
		exit(1);
	}
	return dbResult;



}

void sqlitedb_delete_id(struct sqlitedb *sdb,char * table,int id)
{
	int rc ;
	char * sql;
	char * zErrMsg = 0;
	sql = sqlite3_mprintf("delete from %s where DeviceID=%d",table,id);
	rc = sqlite3_exec(sdb->db,sql,0,0,&zErrMsg);
	sqlite3_free(sql);
}

void sqlitedb_delete_all(struct sqlitedb *sdb,char * table)
{
	char * sql;
	char * zErrMsg = 0;

	sql = sqlite3_mprintf("delete from %s",table);
	sqlite3_exec(sdb->db,sql,0,0,&zErrMsg);
	sqlite3_free(sql);
}

void sqlitedb_updata_data(struct sqlitedb *sdb,int terminalID,char *terminalname)
{
	char *errmsg = NULL;

	char *sql=sqlite3_mprintf("update student set Name='%s' where DeviceID='%d'",terminalname,terminalID);

	if(sqlite3_exec(sdb->db,sql,NULL,NULL,&errmsg) != SQLITE_OK)
	{
		perror("sqlite3_exec_update");
		exit(-1);
	}
	else
		printf("update success!!\n");

}


