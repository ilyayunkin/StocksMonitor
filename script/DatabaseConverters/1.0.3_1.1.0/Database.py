import sqlite3


class Database:
    def __init__(self, filename):
        self.db = sqlite3.connect(filename)
        self.cursor = self.db.cursor()
        
    def execute_query(self, query):
        print(query)
        self.cursor.execute(query)
        
    def query_result(self):
        res = self.cursor.fetchone()[0]
        return res

    def create_table(self, table_name, columns):
        q = "CREATE TABLE " + table_name + ' ' + columns
        self.execute_query(q)
        
    def get_min(self, table_name, column):
        q = 'SELECT min(' + column + ') FROM ' + table_name
        self.execute_query(q)
        minimal = int(self.query_result())
        return minimal

    def get_count(self, table_name):
        q = 'SELECT count(*) FROM ' + table_name;
        self.execute_query(q)
        count = int(self.query_result())
        return count
        
    def get_columns(self, table_name):
        cursor = self.db.execute('select * from ' + table_name)
        names = list(map(lambda x: x[0], cursor.description))
        return names;

    def add_column(self, table_name, column_name):
        self.execute_query("ALTER TABLE " + table_name + " ADD COLUMN " + column_name + ";")
