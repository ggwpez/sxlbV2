#include "log.hpp"
#include "stdio.hpp"
#include "cpuid.hpp"
#include "vga.hpp"
#include "textmode.hpp"
#include <stdarg.h>

static void time_stamp()
{
	if (__32__)
		printf("[%u/%u] ", __STAGE__, cpu::rdtsc());
	else if (__64__)
		printf("[%u/%u] ", __STAGE__, cpu::rdtsc());
	else
		printf("[%u/%u] ", __STAGE__, cpu::rdtsc());
}

static void vlog(char const* fmt, va_list ap, bool print_time = true)
{
	if (print_time)
		time_stamp();

	vprintf(fmt, ap);
}

void log(char const* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vlog(fmt, ap);
	va_end(ap);
}

void logl(char const* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vlog(fmt, ap);
	va_end(ap);

	putchar('\n');
}

static void vlog_war(char const* fmt, va_list ap)
{
	time_stamp();

	vga::tm_t* tm = vga::get_tm();
	vga::tm_t::fc_t save = tm->fc;
	tm->ctrl_set_fc(vga::tm_t::fc_t::BROWN);

	vlog(fmt, ap, false);

	tm->ctrl_set_fc(save);
}

void log_war(char const* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vlog_war(fmt, ap);
	va_end(ap);
}

void logl_war(char const* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vlog_war(fmt, ap);
	va_end(ap);

	putchar('\n');
}

static void vlog_err(char const* fmt, va_list ap)
{
	time_stamp();

	vga::tm_t* tm = vga::get_tm();
	vga::tm_t::fc_t save = tm->fc;
	tm->ctrl_set_fc(vga::tm_t::fc_t::RED);

	vlog(fmt, ap, false);

	tm->ctrl_set_fc(save);
}

void log_err(char const* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vlog_err(fmt, ap);
	va_end(ap);
}

void logl_err(char const* fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	vlog_err(fmt, ap);
	va_end(ap);

	putchar('\n');
}
