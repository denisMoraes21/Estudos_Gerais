package com.example.plaintext.ui.screens.util

import androidx.compose.foundation.clickable
import androidx.compose.foundation.layout.Row
import androidx.compose.foundation.layout.*
import androidx.compose.material3.MaterialTheme
import androidx.compose.material3.Text
import androidx.compose.runtime.Composable
import androidx.compose.ui.Alignment
import androidx.compose.ui.Modifier
import androidx.compose.ui.unit.dp


@Composable
fun PreferenceItem(
    title: String,
    summary: String? = null,
    onClick: (() -> Unit)? = null,
    control: @Composable (() -> Unit)? = null
) {
    Row(
        modifier = Modifier
            .clickable(enabled = onClick != null) { onClick?.invoke() } // Makes it clickable
            .fillMaxWidth()
            .padding(16.dp),
        verticalAlignment = Alignment.CenterVertically
    ) {
        Column(
            modifier = Modifier.weight(1f)
        ) {
            Text(text = title, style = MaterialTheme.typography.titleMedium)
            if (summary != null) {
                Text(text = summary, style = MaterialTheme.typography.bodyMedium)
            }
        }
        if (control != null) {
            control()
        }
    }
}
