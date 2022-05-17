import pyodbc
server = '192.168.101.158/sqlexpress'
database = 'uucontrol'
username = 'uudbms'
password = 'uu@12345'
conn = pyodbc.connect('DRIVER={ODBC Driver 17 for SQL Server};SERVER='+server+';DATABASE='+database+';UID='+username+';PWD='+ password)
cursor = conn.cursor()
cursor.execute('SELECT * FROM dbo.uurawdata')

for row in cursor:
    print(row)
