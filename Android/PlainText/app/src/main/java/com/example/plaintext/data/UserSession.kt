package com.example.plaintext.data

object UserSession {

    var name: String = ""
    var email: String = ""
    var password: String = ""

    fun hasUser(): Boolean {
        return email.isNotEmpty() && password.isNotEmpty()
    }
}