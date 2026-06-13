package com.example.plaintext.utils

import android.os.Build
import android.os.Bundle
import android.os.Parcelable
import androidx.navigation.NavType
import kotlinx.serialization.encodeToString
import kotlinx.serialization.json.Json


/**
 * Cria um tipo de navegação personalizado para objetos Parcelable.
 *
 * @param T O tipo de objeto Parcelable.
 * @param isNullableAllowed Indica se valores nulos são permitidos.
 * @param json Instância do Json para serialização e desserialização.
 * @return Um objeto NavType personalizado para o tipo Parcelable especificado.
 *
 * Este método define um tipo de navegação personalizado que permite a passagem de objetos Parcelable
 * entre destinos de navegação. Ele utiliza a biblioteca de serialização Kotlin para converter os objetos
 * Parcelable em strings JSON e vice-versa. A compatibilidade com diferentes versões do Android é garantida
 * através de verificações de versão.
 */
inline fun <reified T : Parcelable> parcelableType(
    isNullableAllowed: Boolean = false,
    json: Json = Json,
) = object : NavType<T>(isNullableAllowed = isNullableAllowed) {
    override fun get(bundle: Bundle, key: String) =
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.TIRAMISU) {
            bundle.getParcelable(key, T::class.java)
        } else {
            @Suppress("DEPRECATION")
            bundle.getParcelable(key)
        }

    override fun parseValue(value: String): T = json.decodeFromString(value)

    override fun serializeAsValue(value: T): String = json.encodeToString(value)

    override fun put(bundle: Bundle, key: String, value: T) = bundle.putParcelable(key, value)
}