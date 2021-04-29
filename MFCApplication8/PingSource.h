#pragma once


class PingSource
{
private:
	CString m_userCStringInput = _T(" ");

	struct ipAsInteger {
		int part1;
		int part2;
		int part3;
		int part4;
	};

	ipAsInteger m_ipAsFourInt{ 127,0,0,1 };
};
