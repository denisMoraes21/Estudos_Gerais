package com.example.plaintext.ui.viewmodel

import android.app.Application
import android.hardware.Sensor
import android.hardware.SensorEvent
import android.hardware.SensorEventListener
import android.hardware.SensorManager
import android.util.Log
import androidx.compose.runtime.getValue
import androidx.compose.runtime.mutableStateOf
import androidx.compose.runtime.setValue
import androidx.lifecycle.AndroidViewModel
import androidx.lifecycle.viewModelScope
import dagger.hilt.android.lifecycle.HiltViewModel
import kotlinx.coroutines.launch
import javax.inject.Inject

data class SensorData(val x: Float, val y: Float, val z: Float)

data class AllSensorsState(
    val accelerometer: SensorData,
    val gyroscope: SensorData,
    val luminosity: SensorData,
)

data class SensorsStructure(
    val accelerometer: Sensor?,
    val gyroscope: Sensor?,
    val luminosity: Sensor?,
)

@HiltViewModel
class SensorsViewModel @Inject constructor(
    application: Application
) : AndroidViewModel(application),
    SensorEventListener {

    var sensorsState by mutableStateOf(
        AllSensorsState(
            accelerometer = SensorData(0f, 0f, 0f),
            gyroscope = SensorData(0f, 0f, 0f),
            luminosity = SensorData(0f, 0f, 0f),
        )
    )
        private set

    // Sensor manager and accelerometer sensor
    private val sensorManager: SensorManager =
        application.getSystemService(SensorManager::class.java)

    private val sensors: SensorsStructure = SensorsStructure(
        accelerometer = sensorManager.getDefaultSensor(Sensor.TYPE_ACCELEROMETER),
        gyroscope = sensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE),
        luminosity = sensorManager.getDefaultSensor(Sensor.TYPE_LIGHT),
    )

    init {
        // Register the sensor listener
        sensors.accelerometer?.let { sensor ->
            sensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_NORMAL)
        }
        sensors.gyroscope?.let { sensor ->
            sensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_NORMAL)
        }
        sensors.luminosity?.let { sensor ->
            sensorManager.registerListener(this, sensor, SensorManager.SENSOR_DELAY_NORMAL)
        }
    }

    override fun onSensorChanged(event: SensorEvent?) {
        event?.let {
            viewModelScope.launch() {
                // Update the sensor data
                when (it.sensor.type) {
                    Sensor.TYPE_ACCELEROMETER -> sensorsState = sensorsState.copy(
                        accelerometer = SensorData(it.values[0], it.values[1], it.values[2])
                    )

                    Sensor.TYPE_GYROSCOPE -> sensorsState = sensorsState.copy(
                        gyroscope = SensorData(it.values[0], it.values[1], it.values[2])
                    )

                    Sensor.TYPE_LIGHT -> sensorsState = sensorsState.copy(
                        luminosity = SensorData(it.values[0], 0f, 0f)
                    )
                }
            }
        }
    }


    override fun onAccuracyChanged(sensor: Sensor?, accuracy: Int) {
        // Do nothing
    }
}

