#include "ps4.h"
#include "patch.h"

int nthread_run;
char notify_buf[1024];

void systemMessage(char* msg) {
 	char buffer[512]; 
 	sprintf(buffer, "%s\n\n\n\n\n\n\n", msg);
 	sceSysUtilSendSystemNotificationWithText(0x81, buffer);
}

void copyFile(char *sourcefile, char* destfile)
{
    FILE *src = fopen(sourcefile, "rb");
    if (src)
    {
        FILE *out = fopen(destfile,"wb");
        if (out)
        {
            size_t bytes;
            char *buffer = malloc(65536);
            if (buffer != NULL)
            {
                while (0 < (bytes = fread(buffer, 1, 65536, src)))
                    fwrite(buffer, 1, bytes, out);
                    free(buffer);
            }
            fclose(out);
        }
        else {
        }
        fclose(src);
    }
    else {
    }
}


void copyDir(char *sourcedir, char* destdir)
{
    DIR *dir;
    struct dirent *dp;
    struct stat info;
    char src_path[1024], dst_path[1024];

    dir = opendir(sourcedir);
    if (!dir)
        return;
        mkdir(destdir, 0777);
    while ((dp = readdir(dir)) != NULL)
    {
        if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
        {}
        else
        {
            sprintf(src_path, "%s/%s", sourcedir, dp->d_name);
            sprintf(dst_path, "%s/%s", destdir  , dp->d_name);

            if (!stat(src_path, &info))
            {
                if (S_ISDIR(info.st_mode))
                {
                  copyDir(src_path, dst_path);
                }
                else
                if (S_ISREG(info.st_mode))
                {
                  copyFile(src_path, dst_path);
                }
            }
        }
    }
    closedir(dir);
}


void *nthread_func(void *arg)
{
        time_t t1, t2;
        t1 = 0;
	while (nthread_run)
	{
		if (notify_buf[0])
		{
			t2 = time(NULL);
			if ((t2 - t1) >= 15)
			{
				t1 = t2;
                                systemMessage(notify_buf);
			}
		}
		else t1 = 0;
		sceKernelSleep(1);
	}

	return NULL;
}



int _main(struct thread *td) {
      initKernel();
      initLibc();
      initPthread();
      DIR *dir;
      dir = opendir("/system_data/savedata");
      if (!dir)
      {
       syscall(11,patcher,td);
      }
      else
      {
       closedir(dir);
      }

      initSysUtil();
      nthread_run = 1;
      notify_buf[0] = '\0';
      ScePthread nthread;
      scePthreadCreate(&nthread, NULL, nthread_func, NULL, "nthread");
      FILE *usbdir = fopen("/mnt/usb0/.dirtest", "wb");
    
         if (!usbdir)
            {
                usbdir = fopen("/mnt/usb1/.dirtest", "wb");
                if (!usbdir)
                {
                       	copyFile("/system_data/priv/mms/app.db", "/system_data/priv/mms/app.db_backup");
                        copyFile("/system_data/priv/mms/addcont.db", "/system_data/priv/mms/addcont.db_backup");
                        systemMessage("Internal backup complete.\nThis was only a database backup use a usb drive for full backup.");
                        nthread_run = 0;
                        return 0;
                }
                else
                {
                        fclose(usbdir);
                        systemMessage("Backing up to USB1");
                        unlink("/mnt/usb1/.dirtest");
                        mkdir("/mnt/usb1/DB_Dackup/", 0777);
	                copyFile("/system_data/priv/mms/app.db", "/mnt/usb1/DB_Dackup/app.db");
	                copyFile("/system_data/priv/mms/addcont.db", "/mnt/usb1/DB_Dackup/addcont.db");
                        mkdir("/mnt/usb1/GameSaves/", 0777);
                        mkdir("/mnt/usb1/GameSaves/system_data/", 0777);
                        mkdir("/mnt/usb1/GameSaves/system_data/savedata", 0777);
                        mkdir("/mnt/usb1/GameSaves/user/", 0777);
                        mkdir("/mnt/usb1/GameSaves/user/home/", 0777);
                        sprintf(notify_buf, "Copying: Game Saves\nPlease wait.");
                        copyDir("/system_data/savedata","/mnt/usb1/GameSaves/system_data/savedata");
                        sprintf(notify_buf, "Copying: User data\nPlease wait.");
                        copyDir("/user/home", "/mnt/usb1/GameSaves/user/home");
                        notify_buf[0] = '\0';
                        nthread_run = 0;
                        systemMessage("USB Backup Complete.");

                }
            }
            else
            {
                        fclose(usbdir);
                        systemMessage("Backing up to USB0");
                        unlink("/mnt/usb0/.dirtest");
                        mkdir("/mnt/usb0/DB_Dackup/", 0777);
	                copyFile("/system_data/priv/mms/app.db", "/mnt/usb0/DB_Dackup/app.db");
	                copyFile("/system_data/priv/mms/addcont.db", "/mnt/usb0/DB_Dackup/addcont.db");
                        mkdir("/mnt/usb0/GameSaves/", 0777);
                        mkdir("/mnt/usb0/GameSaves/system_data/", 0777);
                        mkdir("/mnt/usb0/GameSaves/system_data/savedata", 0777);
                        mkdir("/mnt/usb0/GameSaves/user/", 0777);
                        mkdir("/mnt/usb0/GameSaves/user/home/", 0777);
                        sprintf(notify_buf, "Copying: Game Saves\nPlease wait.");
                        copyDir("/system_data/savedata","/mnt/usb0/GameSaves/system_data/savedata");
                        sprintf(notify_buf, "Copying: User data\nPlease wait.");
                        copyDir("/user/home", "/mnt/usb0/GameSaves/user/home");
                        notify_buf[0] = '\0';
                        nthread_run = 0;
                        systemMessage("USB Backup Complete.");
            }
    
    return 0;
}


