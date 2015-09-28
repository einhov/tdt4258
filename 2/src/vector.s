.section .vector

	.long stack_top
	.long _start
	.long dummy_handler           /* NMI Handler                  */
	.long dummy_handler           /* Hard Fault Handler           */
	.long dummy_handler           /* MPU Fault Handler            */
	.long dummy_handler           /* Bus Fault Handler            */
	.long dummy_handler           /* Usage Fault Handler          */
	.long dummy_handler           /* Reserved                     */
	.long dummy_handler           /* Reserved                     */
	.long dummy_handler           /* Reserved                     */
	.long dummy_handler           /* Reserved                     */
	.long dummy_handler           /* SVCall Handler               */
	.long dummy_handler           /* Debug Monitor Handler        */
	.long dummy_handler           /* Reserved                     */
	.long dummy_handler           /* PendSV Handler               */
	.long dummy_handler           /* SysTick Handler              */

	.long dummy_handler
	.long gpio_handler            /* GPIO even handler */
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long gpio_handler            /* GPIO odd handler */
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long letimer0_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
	.long dummy_handler
