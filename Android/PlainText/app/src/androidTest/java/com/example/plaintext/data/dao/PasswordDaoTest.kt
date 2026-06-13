package com.example.plaintext.data.dao

import android.content.Context
import androidx.room.Room
import androidx.test.core.app.ApplicationProvider
import androidx.test.ext.junit.runners.AndroidJUnit4
import com.example.plaintext.data.PlainTextDatabase
import com.example.plaintext.data.model.Password
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.runBlocking
import org.junit.After
import org.junit.Assert.assertEquals
import org.junit.Before
import org.junit.Test
import org.junit.runner.RunWith
import java.io.IOException

@RunWith(AndroidJUnit4::class)
class PasswordDaoTest {

    private lateinit var passwordDao: PasswordDao
    private lateinit var db: PlainTextDatabase

    @Before
    fun createDb() {
        val context = ApplicationProvider.getApplicationContext<Context>()
        db = Room.inMemoryDatabaseBuilder(
            context, PlainTextDatabase::class.java
        ).allowMainThreadQueries().build()
        passwordDao = db.passwordDao()
    }

    @After
    @Throws(IOException::class)
    fun closeDb() {
        db.close()
    }

    @Test
    @Throws(Exception::class)
    fun testFirstRegister() = runBlocking {

        // Register creation
        val password = Password(
            1,
            "Denis",
            "denis",
            "example123",
            "user_test")

        // Add register on database table
        passwordDao.insert(password)

        // Check get all
        val allPasswords = passwordDao.getAll().first()

        assertEquals(allPasswords[0], password)
    }

    @Test
    @Throws(Exception::class)
    fun testGetEmptyTable() = runBlocking {

        // Check get all
        val allPasswords = passwordDao.getAll().first()

        assertEquals(0, allPasswords.size)
    }

    @Test
    @Throws(Exception::class)
    fun testGetMoreThanOneRegister() = runBlocking {

        // First register
        val p1 = Password(
            1,
            "Name 1",
            "user1",
            "pass1"
        )

        // Second register
        val p2 = Password(
            2,
            "Name 2",
            "user2",
            "pass2"
        )

        // Insert two registers
        passwordDao.insert(p1)
        passwordDao.insert(p2)

        // Get registers
        val allPasswords = passwordDao.getAll().first()

        assertEquals(2, allPasswords.size)
        assertEquals(p1, allPasswords.find { it.id == 1 })
        assertEquals(p2, allPasswords.find { it.id == 2 })
    }

    @Test
    @Throws(Exception::class)
    fun testGetByName() = runBlocking {
        val p1 = Password(
            1,
            "Name 1",
            "user1",
            "pass1"
        )
        passwordDao.insert(p1)
        val register = passwordDao.getByName("Name 1").first()

        assertEquals("Name 1", register!!.name)
        assertEquals(1, register.id)
        assertEquals("user1", register.login)
        assertEquals("pass1", register.password)


    }
}
