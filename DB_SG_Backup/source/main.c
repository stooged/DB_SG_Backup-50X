#include "ps4.h"
#include "patch.h"

int nthread_run;
char notify_buf[1024];

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
      syscall(11,patcher,td);
      initSysUtil();
      nthread_run = 1;
      notify_buf[0] = '\0';
      ScePthread nthread;
      scePthreadCreate(&nthread, NULL, nthread_func, NULL, "nthread");
      int usbdir = open("/mnt/usb0/.dirtest", O_WRONLY | O_CREAT | O_TRUNC, 0777);
         if (usbdir == -1)
            {
                usbdir = open("/mnt/usb1/.dirtest", O_WRONLY | O_CREAT | O_TRUNC, 0777);
                if (usbdir == -1)
                {
                       	copy_File("/system_data/priv/mms/app.db", "/system_data/priv/mms/app.db_backup");
                        copy_File("/system_data/priv/mms/addcont.db", "/system_data/priv/mms/addcont.db_backup");
                        copy_File("/system_data/priv/mms/av_content_bg.db", "/system_data/priv/mms/av_content_bg.db_backup");
                        copy_File("/user/system/webkit/secure/appcache/ApplicationCache.db", "/user/system/webkit/secure/appcache/ApplicationCache.db_backup");
						copy_File("/user/system/webkit/webbrowser/appcache/ApplicationCache.db", "/user/system/webkit/webbrowser/appcache/ApplicationCache.db_backup");
                        systemMessage("Internal backup complete.\nThis was only a database backup use a usb drive for full backup.");
                        nthread_run = 0;
                        return 0;
                }
                else
                {
                        close(usbdir);
                        systemMessage("Backing up to USB1");
                        unlink("/mnt/usb1/.dirtest");
                        mkdir("/mnt/usb1/DB_Dackup/", 0777);
                        copy_File("/system_data/priv/mms/app.db", "/mnt/usb1/DB_Dackup/app.db");
                        copy_File("/system_data/priv/mms/addcont.db", "/mnt/usb1/DB_Dackup/addcont.db");
                        copy_File("/system_data/priv/mms/av_content_bg.db", "/mnt/usb1/DB_Dackup/av_content_bg.db");
                        mkdir("/mnt/usb1/UserData/", 0777);
                        mkdir("/mnt/usb1/UserData/system_data/", 0777);
                        mkdir("/mnt/usb1/UserData/system_data/savedata", 0777);
                        mkdir("/mnt/usb1/UserData/system_data/priv", 0777);
                        mkdir("/mnt/usb1/UserData/system_data/priv/home", 0777);
                        mkdir("/mnt/usb1/UserData/system_data/priv/license", 0777);
                        mkdir("/mnt/usb1/UserData/system_data/priv/activation", 0777);
                        mkdir("/mnt/usb1/UserData/user/", 0777);
                        mkdir("/mnt/usb1/UserData/user/home/", 0777);
                        mkdir("/mnt/usb1/UserData/user/trophy", 0777);
                        mkdir("/mnt/usb1/UserData/user/license", 0777);
                        mkdir("/mnt/usb1/UserData/user/settings", 0777);
						mkdir("/mnt/usb1/UserData/user/system", 0777);					
						mkdir("/mnt/usb1/UserData/user/system/webkit", 0777);
						mkdir("/mnt/usb1/UserData/user/system/webkit/secure", 0777);
						mkdir("/mnt/usb1/UserData/user/system/webkit/webbrowser", 0777);
                        sprintf(notify_buf, "Copying: User Data\nPlease wait.");
                        copy_Dir("/system_data/savedata","/mnt/usb1/UserData/system_data/savedata");
                        copy_Dir("/user/home", "/mnt/usb1/UserData/user/home");
                        copy_Dir("/user/trophy", "/mnt/usb1/UserData/user/trophy");
                        copy_Dir("/user/license", "/mnt/usb1/UserData/user/license");
                        copy_Dir("/user/settings", "/mnt/usb1/UserData/user/settings");
						copy_Dir("/user/system/webkit/secure","/mnt/usb1/UserData/user/system/webkit/secure");
						copy_Dir("/user/system/webkit/webbrowser","/mnt/usb1/UserData/user/system/webkit/webbrowser");
                        copy_Dir("/system_data/priv/home","/mnt/usb1/UserData/system_data/priv/home");
                        copy_Dir("/system_data/priv/license","/mnt/usb1/UserData/system_data/priv/license");
                        copy_Dir("/system_data/priv/activation","/mnt/usb1/UserData/system_data/priv/activation");
                        notify_buf[0] = '\0';
                        nthread_run = 0;
                        systemMessage("USB Backup Complete.");
                }
            }
            else
            {
                        close(usbdir);
                        systemMessage("Backing up to USB0");
                        unlink("/mnt/usb0/.dirtest");
                        mkdir("/mnt/usb0/DB_Dackup/", 0777);
                        copy_File("/system_data/priv/mms/app.db", "/mnt/usb0/DB_Dackup/app.db");
                        copy_File("/system_data/priv/mms/addcont.db", "/mnt/usb0/DB_Dackup/addcont.db");
                        copy_File("/system_data/priv/mms/av_content_bg.db", "/mnt/usb0/DB_Dackup/av_content_bg.db");
                        mkdir("/mnt/usb0/UserData/", 0777);
                        mkdir("/mnt/usb0/UserData/system_data/", 0777);
                        mkdir("/mnt/usb0/UserData/system_data/savedata", 0777);
                        mkdir("/mnt/usb0/UserData/system_data/priv", 0777);
                        mkdir("/mnt/usb0/UserData/system_data/priv/home", 0777);
                        mkdir("/mnt/usb0/UserData/system_data/priv/license", 0777);
                        mkdir("/mnt/usb0/UserData/system_data/priv/activation", 0777);
                        mkdir("/mnt/usb0/UserData/user/", 0777);
                        mkdir("/mnt/usb0/UserData/user/home/", 0777);
                        mkdir("/mnt/usb0/UserData/user/trophy", 0777);
                        mkdir("/mnt/usb0/UserData/user/license", 0777);
                        mkdir("/mnt/usb0/UserData/user/settings", 0777);
						mkdir("/mnt/usb0/UserData/user/system", 0777);					
						mkdir("/mnt/usb0/UserData/user/system/webkit", 0777);
						mkdir("/mnt/usb0/UserData/user/system/webkit/secure", 0777);
						mkdir("/mnt/usb0/UserData/user/system/webkit/webbrowser", 0777);
                        sprintf(notify_buf, "Copying: User Data\nPlease wait.");
                        copy_Dir("/system_data/savedata","/mnt/usb0/UserData/system_data/savedata");
                        copy_Dir("/user/home", "/mnt/usb0/UserData/user/home");
                        copy_Dir("/user/trophy", "/mnt/usb0/UserData/user/trophy");
                        copy_Dir("/user/license", "/mnt/usb0/UserData/user/license");
                        copy_Dir("/user/settings", "/mnt/usb0/UserData/user/settings");
						copy_Dir("/user/system/webkit/secure","/mnt/usb0/UserData/user/system/webkit/secure");
						copy_Dir("/user/system/webkit/webbrowser","/mnt/usb0/UserData/user/system/webkit/webbrowser");
                        copy_Dir("/system_data/priv/home","/mnt/usb0/UserData/system_data/priv/home");
                        copy_Dir("/system_data/priv/license","/mnt/usb0/UserData/system_data/priv/license");
                        copy_Dir("/system_data/priv/activation","/mnt/usb0/UserData/system_data/priv/activation");
                        notify_buf[0] = '\0';
                        nthread_run = 0;
                        systemMessage("USB Backup Complete.");
            }
    
    return 0;
}


