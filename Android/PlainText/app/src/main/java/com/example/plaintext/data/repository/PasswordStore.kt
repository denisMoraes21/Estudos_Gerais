package com.example.plaintext.data.repository

import com.example.plaintext.data.dao.PasswordDao
import com.example.plaintext.data.model.Password
import com.example.plaintext.data.model.PasswordInfo
import kotlinx.coroutines.flow.Flow
import kotlinx.coroutines.flow.first
import kotlinx.coroutines.flow.map

interface PasswordDBStore {
    fun getList(): Flow<List<Password>>
    suspend fun add(password: Password): Long
    suspend fun update(password: Password)
    suspend fun get(id: Int): Password?
    suspend fun save(passwordInfo: PasswordInfo)
    suspend fun isEmpty(): Flow<Boolean>
    fun login(login: String, password: String): Flow<Password?>
    suspend fun delete(password: Password)
}

class LocalPasswordDBStore(
    private val passwordDao : PasswordDao
): PasswordDBStore {
    override fun getList(): Flow<List<Password>> {
        return passwordDao.getAll()
    }

    override suspend fun add(password: Password): Long {
        return passwordDao.insert(password)
    }

    override suspend fun update(password: Password) {
        return passwordDao.update(password)
    }

    override suspend fun get(id: Int): Password? {
        return passwordDao.getByID(id)
    }

    override suspend fun save(passwordInfo: PasswordInfo) {
        val password by passwordInfo
        passwordDao.insert(password)
    }

    override suspend fun isEmpty(): Flow<Boolean> {
        return passwordDao.getAll().map { passwords ->
            passwords.isEmpty()
        }
    }

    override fun login(login: String, password: String): Flow<Password?> {
        return passwordDao.getByLoginAndPassword(login, password)
    }

    override suspend fun delete(password: Password) {
        passwordDao.delete(password)
    }

}