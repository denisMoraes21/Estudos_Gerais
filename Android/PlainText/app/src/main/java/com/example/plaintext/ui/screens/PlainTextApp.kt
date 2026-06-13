package com.example.plaintext.ui.screens

import androidx.compose.foundation.layout.padding
import androidx.compose.material3.Scaffold
import androidx.compose.runtime.Composable
import androidx.compose.ui.Modifier
import androidx.hilt.navigation.compose.hiltViewModel
import androidx.lifecycle.viewmodel.compose.viewModel
import androidx.navigation.compose.NavHost
import androidx.navigation.compose.composable
import androidx.navigation.compose.currentBackStackEntryAsState
import androidx.navigation.toRoute
import com.example.plaintext.data.model.PasswordInfo
import com.example.plaintext.ui.screens.editList.EditList
import com.example.plaintext.ui.screens.hello.Hello_screen
import com.example.plaintext.ui.screens.list.AddButton
import com.example.plaintext.ui.screens.list.ListView
import com.example.plaintext.ui.screens.login.Login_screen
import com.example.plaintext.ui.screens.login.TopBarComponent
import com.example.plaintext.ui.screens.preferences.SettingsScreen
import com.example.plaintext.ui.viewmodel.ListViewModel
import com.example.plaintext.ui.viewmodel.PreferencesViewModel
import com.example.plaintext.utils.parcelableType
import kotlin.reflect.typeOf
import androidx.compose.material3.Text
import com.example.plaintext.ui.screens.register.RegisterScreen

@Composable
fun PlainTextApp(
    appState: JetcasterAppState = rememberJetcasterAppState()
) {
    NavHost(
        navController = appState.navController,
//        startDestination = Screen.Hello("DevTITANS"),
        startDestination = Screen.Login,
    )
    {
        composable<Screen.Hello>{
            var args = it.toRoute<Screen.Hello>()
            Hello_screen(args)
        }

        composable<Screen.Login>{
            Login_screen(
                navigateToSettings = appState::navigateToPreferences,
                navigateToList = appState::navigateToList,
                navigateToRegister = {
                    appState.navController.navigate(Screen.Register)
                }
            )
        }

        composable<Screen.Preferences> {
            SettingsScreen(
                navController = appState.navController
            )
        }
        composable<Screen.Register> {
            RegisterScreen(
                navigateBack = {
                    appState.navController.popBackStack()
                }
            )
        }
        composable<Screen.List> {
            val viewModel: ListViewModel = hiltViewModel()
            ListView(
                viewModel = viewModel,
                navigateToEdit = { password ->
                    appState.navController.navigate(
                        Screen.EditList(password)
                    )
                },
                navigateToSettings = appState::navigateToPreferences
            )
        }

        composable<Screen.EditList>(
            typeMap = mapOf(typeOf<PasswordInfo>() to parcelableType<PasswordInfo>())
        ) { it ->
            val args = it.toRoute<Screen.EditList>()
            val viewModel: ListViewModel = hiltViewModel()
            EditList(
                args = it.toRoute(),
                navigateBack = { appState.navController.popBackStack() },
                savePassword = { viewModel.savePassword(it) },
                deletePassword = { viewModel.deletePassword(it) }
            )
        }
    }
}