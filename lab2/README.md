# Лабораторная работа 2

**Название:** "Разработка драйверов блочных устройств"

**Цель работы:** Получить знания и навыки разработки драйверов блочных устройств для операционной системы Linux.

## Описание функциональности драйвера

Драйвер создаёт виртуальный жёсткий диск в оперативной памяти размером 50 Мбайт и разбивает его на 2 первичных и 1 расширенный разделы с размерами 10Мбайт, 20Мбайт и 20Мбайт соответственно. Расширяемый раздел разделен на 2 логических по 10Мбайт каждый. Каждый последующий байт является суммой предыдущих.

## Инструкция по сборке

make install - сборка и загрузка драйвера \n
make remove - выгрузка драйвера \n

## Инструкция пользователя
После загрузки модуля появятся файлы /dev/vramdisk и /dev/vramdisk<1-6>. Записать информацию на диск можно с помощью команды dd. Считать информацию можно с помощью команды hexdump.

## Примеры использования
**Вывод разделов созданного виртуального диска**
```shell
sgtminus@Ubuntu20:~/Desktop/lab2$ fdisk -l /dev/vramdisk  
Disk /dev/vramdisk: 50 MiB, 52428800 bytes, 102400 sectors  
Units: sectors of 1 * 512 = 512 bytes  
Sector size (logical/physical): 512 bytes / 512 bytes  
I/O size (minimum/optimal): 512 bytes / 512 bytes  
Disklabel type: dos  
Disk identifier: 0x36e5756d  

Device         Boot Start    End Sectors Size Id Type  
/dev/vramdisk1          1  20480   20480  10M 83 Linux  
/dev/vramdisk2      20481  61440   40960  20M 83 Linux  
/dev/vramdisk3      61441 102402   40962  20M  5 Extended  
/dev/vramdisk5      61442  81921   20480  10M 83 Linux  
/dev/vramdisk6      81923 102402   20480  10M 83 Linux  
```

**Запись и чтение данных в созданный виртуальный диск**
```shell
sgtminus@Ubuntu20:~/Desktop/lab2$ echo "test test" > testfile
sgtminus@Ubuntu20:~/Desktop/lab2$ sudo dd if=testfile of=/dev/vramdisk2 count=1
0+1 records in
0+1 records out
10 bytes copied, 0.000136595 s, 73.2 kB/s
```

```shell
sgtminus@Ubuntu20:~/Desktop/lab2$ sudo hexdump /dev/vramdisk2
0000000 0000 0000 0000 0000 0000 0000 0000 0000
*
1400000
```

**Измерение скорости передачи данных при копировании файлов между разделами созданного виртуального диска**

```shell
sgtminus@Ubuntu20:~/Desktop/lab2$ sudo dd if=/dev/vramdisk1 of=/dev/vramdisk2
20480+0 records in
20480+0 records out
10485760 bytes (10 MB, 10 MiB) copied, 0.0729483 s, 144 MB/s
```

**Измерение скорости передачи данных при копировании файлов между разделами виртуального и реального жестких дисков**

```shell
sgtminus@Ubuntu20:~/Desktop/lab2$ sudo dd if=/dev/vramdisk1 of=/dev/sda3
20480+0 records in
20480+0 records out
10485760 bytes (10 MB, 10 MiB) copied, 0.0258481 s, 406 MB/s
```

```shell
sgtminus@Ubuntu20:~/Desktop/lab2$ sudo dd of=/dev/vramdisk1 if=/dev/sda3
20480+0 records in
20480+0 records out
10485760 bytes (10 MB, 10 MiB) copied, 0.0424674 s, 247 MB/s
```

