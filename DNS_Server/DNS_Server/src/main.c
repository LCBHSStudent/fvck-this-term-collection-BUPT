#include <config.h>
#include <hash_table.h>

static IDTable	idTable;
static DNSTable dnsTable;

static size_t   idRowCount  = 0;
static size_t   dnsRowCount = 0;


static time_t	sysTime;
static TIME		sysTimeLocal;

static char		url[LENGTH];

int main(int argc, char* argv[]) {
	char a[11] = "0123456789";
	printf("%lld", ELFHash(a, sizeof(a) - 1));
	return 0;
}