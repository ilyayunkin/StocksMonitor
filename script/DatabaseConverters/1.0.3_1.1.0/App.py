from Database import Database

db = Database('pocket.sqlite')
columns = db.get_columns("Pocket")
print("Extisting columns: " + str(columns))
if "sell_price" not in columns:
    db.add_column('Pocket', 'sell_price')
else:
    print("Nothing to change")
