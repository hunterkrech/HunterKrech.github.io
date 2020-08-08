package hunterkrech.cs360.campsiteloacator;

import android.content.ContentValues;
import android.content.Context;
import android.database.ContentObservable;
import android.database.Cursor;
import android.database.sqlite.SQLiteOpenHelper;
import android.database.sqlite.SQLiteDatabase;

public class DogDBHandler extends SQLiteOpenHelper{
    // database name and version
    private static final int DB_VER = 1;
    private static final String DB_NAME = "dogDB.db";
    // table
    public static final String TABLE_DOGS = "dogs";
    // columns
    public static final String COLUMN_ID = "id";
    public static final String COLUMN_NAME = "name";
    public static final String COLUMN_AGE = "age";
    // constructor
    public DogDBHandler(Context context, String name,
                        SQLiteDatabase.CursorFactory factory, int version)
    {
        super(context, DB_NAME, factory, DB_VER);
    }
    // This method creates the Dogs table when the DB is initialized.
    @Override
    public void onCreate(SQLiteDatabase db) {
        String CREATE_DOGS_TABLE = "CREATE TABLE " +
                TABLE_DOGS + "(" +
                COLUMN_ID + " INTEGER PRIMARY KEY," +
                COLUMN_NAME + " TEXT," +
                COLUMN_AGE + " INTEGER" + ")";
        db.execSQL(CREATE_DOGS_TABLE);
    }
    // This method closes an open DB if a new one is created.
    @Override
    public void onUpgrade(SQLiteDatabase db, int oldVersion, int newVersion) {
        db.execSQL("DROP TABLE IF EXISTS " + TABLE_DOGS);
        onCreate(db);
    }
    // This method is used to add a Dog record to the database.
    public void addDog(Dog dog) {
        ContentValues values = new ContentValues();
        values.put(COLUMN_NAME, dog.getName());
        values.put(COLUMN_AGE, dog.getAge());
        SQLiteDatabase db = this.getWritableDatabase();
        db.insert(TABLE_DOGS, null, values);
        db.close();
    }
    // implements the search/find functionality
    public Dog searchDog(String dogName) {
        String query = "SELECT * FROM " +
                TABLE_DOGS + " WHERE " + COLUMN_NAME +
                " = \"" + dogName + "\"";
        SQLiteDatabase db = this.getWritableDatabase();
        Cursor cursor = db.rawQuery(query, null);
        Dog dog = new Dog();
        if (cursor.moveToFirst()) {
            cursor.moveToFirst();
            dog.setID(Integer.parseInt(cursor.getString(0)));
            dog.setName(cursor.getString(1));
            dog.setAge(Integer.parseInt(cursor.getString(2)));
            cursor.close();
        } else {
            dog = null;
        }
        db.close();
        return dog;
    }
    // implements the delete dog functionality
    public boolean deleteDog(String dogName) {
        boolean result = false;
        String query = "SELECT * FROM " + TABLE_DOGS +
                " WHERE " + COLUMN_NAME + " = \"" + dogName + "\"";
        SQLiteDatabase db = this.getWritableDatabase();
        Cursor cursor = db.rawQuery(query, null);
        Dog dog = new Dog();
        if (cursor.moveToFirst()) {
            dog.setID(Integer.parseInt(cursor.getString(0)));
            db.delete(TABLE_DOGS, COLUMN_ID + " = ?",
                    new String[] { String.valueOf(dog.getID())});
            cursor.close();
            result = true;
        }
        db.close();
        return result;
    }
}