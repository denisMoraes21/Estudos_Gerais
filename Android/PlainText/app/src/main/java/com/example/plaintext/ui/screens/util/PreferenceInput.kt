package com.example.plaintext.ui.screens.util

import androidx.compose.runtime.Composable
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.remember

/**
 * Componente de entrada de preferência que exibe um item de preferência e, ao ser clicado,
 * abre um diálogo de entrada para o usuário inserir um valor.
 *
 * @param title O título do item de preferência.
 * @param label O rótulo do campo de entrada no diálogo.
 * @param summary O resumo do item de preferência.
 * @param fieldValue O valor atual do campo de entrada (opcional).
 * @param onFinish Função de callback que é chamada quando o usuário finaliza a entrada no diálogo.
 *
 * Este componente utiliza um estado lembrado para controlar a exibição do diálogo de entrada.
 * Quando o item de preferência é clicado, o diálogo é exibido permitindo ao usuário inserir um valor.
 * Após a inserção, a função de callback `onFinish` é chamada com o valor inserido.
 */
@Composable
fun PreferenceInput(
    title: String,
    label: String,
    summary: String,
    fieldValue: String? = null,
    onFinish: ((field:String) -> Unit) = {} // trailing lambda - can receive a function inside curly braces
){
    val shouldShowLoginDialog = remember { mutableStateOf(false) }

    if (shouldShowLoginDialog.value) {
        MyInputAlertDialog(shouldShowDialog = shouldShowLoginDialog, title, label, fieldValue, onFinish = onFinish)
    }

    PreferenceItem(
        title = title,
        summary = summary,
        onClick = { shouldShowLoginDialog.value = true }
    )
}