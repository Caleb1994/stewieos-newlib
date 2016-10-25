#ifndef _STEWIEOS_MNTENT_H_
#define _STEWIEOS_MNTENT_H_

#define MNTTYPE_IGNORE "ignore"
#define MNTTYPE_NFS "nfs"
#define MNTTYPE_SWAP "swap"

#define MNTOPT_DEFAULTS "defaults"
#define MNTOPT_RO "ro"
#define MNTOPT_RW "rw"
#define MNTOPT_SUID "suid"
#define MNTOPT_NOSUID "nosuid"
#define MNTOPT_NOAUTO "noauto"

struct mntent
{
	char* mnt_fsname;
	char* mnt_dir;
	char* mnt_type;
	char* mnt_opts;
	int mnt_freq;
	int mnt_passno; 
};

FILE* setmntent(const char* filename, const char* mode);
struct mntent* getmntent(FILE* stream);
int addmntent(FILE* stream, const struct mntent* mnt);
char* hasmntopt(const struct mntent* mnt, const char* opt);
int endmntent(FILE* stream);

#endif