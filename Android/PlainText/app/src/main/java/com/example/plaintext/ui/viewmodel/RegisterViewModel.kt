package com.example.plaintext.ui.viewmodel

import androidx.lifecycle.ViewModel
import androidx.lifecycle.viewModelScope
import com.example.plaintext.data.model.Password
import com.example.plaintext.data.repository.PasswordDBStore
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

@HiltViewModel
class RegisterViewModel @Inject constructor(
    private val store: PasswordDBStore
) : ViewModel() {

    fun register(name: String, email: String, password: String) {
        viewModelScope.launch {
            store.add(
                Password(
                    id = 0,
                    name = name,
                    login = email,
                    password = password
                )
            )
        }
    }
}