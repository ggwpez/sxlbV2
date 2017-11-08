#pragma once

#define MAX_CBS 32

/// Use this for ALL OR NOTHING commits
/// ex.
///
/// a();
/// b();
/// c();
///
/// If c() failes, we need to rollback a() with a_inverse() and b() with b_inverse():
///
/// a();
/// rollback_collector.add(a_inverse);
/// b()
/// rollback_collector.add(b_inverse);
/// c()
///
class rollback_collector
{
	rollback_collector() = default;
};
