# Run with CM7 paused and CM7/build/test_CM7.elf symbols loaded.
# Reads target state only: no reset, resume or calls into the firmware.
set pagination off
echo \n--- CPU and current task ---\n
bt
info registers pc lr sp xpsr
p pxCurrentTCB->pcTaskName
p xTickCount
echo \n--- Fault registers (live) and FreeRTOS hooks ---\n
x/6wx 0xE000ED28
p freertos_failure_reason
p freertos_failed_task
echo \n--- Ethernet state ---\n
p heth.gState
p/x heth.ErrorCode
p/x heth.DMAErrorCode
p/x heth.Instance->DMACSR
p heth.TxDescList
p eth_irq_count
p eth_rx_complete_count
p eth_tx_complete_count
echo \n--- Mutex owners (task handles) ---\n
p ((Queue_t *)TxMutex)->u.xSemaphore.xMutexHolder
p ((Queue_t *)lwip_sys_mutex)->u.xSemaphore.xMutexHolder
p ((Queue_t *)lock_tcpip_core)->u.xSemaphore.xMutexHolder
echo \n--- Link callback completion semaphore ---\n
p ((Queue_t *)LinkUpdateDone)->uxMessagesWaiting
