package com.example.plaintext.data
import com.example.plaintext.data.model.PasswordInfo

object PasswordMemoryStore {

    private val passwords = mutableListOf<PasswordInfo>()

    fun getAll(): List<PasswordInfo> {
        return passwords
    }

    fun add(password: PasswordInfo) {
        val index = passwords.indexOfFirst { it.id == password.id }
        if (index != -1) {
            passwords[index] = password
        } else {
            val newId = if (passwords.isEmpty()) 1 else passwords.maxOf { it.id } + 1
            passwords.add(password.copy(id = newId))
        }
    }

    fun delete(id: Int) {
        passwords.removeAll { it.id == id }
    }
}
