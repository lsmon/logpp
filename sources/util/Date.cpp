// MIT License
//
// Copyright (c) 2023 Salomon Lee
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
//

#include <util/Date.hpp>
#include <sstream>
#include <sys/time.h>
#ifdef __linux__
#include <chrono>
#endif
#define SEC_TO_USEC 1000000
#define MIN_TO_SEC 60
#define HOUR_TO_SEC 3600
#define DAY_TO_SEC 86400
#define MONTH_TO_SEC 2629743
#define YEAR_TO_SEC 31556926

Date::Date() {
    m_time_zone["BST"] = "0100";
    m_time_zone["CET"] = "0100";
    m_time_zone["DFT"] = "0100";
    m_time_zone["IST"] = "0100";
    m_time_zone["MET"] = "0100";
    m_time_zone["WAT"] = "0100";
    m_time_zone["WEST"] = "0100";
    m_time_zone["CAT"] = "0200";
    m_time_zone["CEST"] = "0200";
    m_time_zone["EET"] = "0200";
    m_time_zone["HAEC"] = "0200";
    m_time_zone["IST"] = "0200";
    m_time_zone["KALT"] = "0200";
    m_time_zone["MEST"] = "0200";
    m_time_zone["SAST"] = "0200";
    m_time_zone["WAST"] = "0200";
    m_time_zone["AST"] = "0300";
    m_time_zone["EAT"] = "0300";
    m_time_zone["EEST"] = "0300";
    m_time_zone["FET"] = "0300";
    m_time_zone["IDT"] = "0300";
    m_time_zone["IOT"] = "0300";
    m_time_zone["MSK"] = "0300";
    m_time_zone["SYOT"] = "0300";
    m_time_zone["TRT"] = "0300";
    m_time_zone["IRST"] = "0330";
    m_time_zone["AMT"] = "0400";
    m_time_zone["AZT"] = "0400";
    m_time_zone["GET"] = "0400";
    m_time_zone["GST"] = "0400";
    m_time_zone["MUT"] = "0400";
    m_time_zone["RET"] = "0400";
    m_time_zone["SAMT"] = "0400";
    m_time_zone["SCT"] = "0400";
    m_time_zone["VOLT"] = "0400";
    m_time_zone["AFT"] = "0430";
    m_time_zone["IRDT"] = "0430";
    m_time_zone["AQTT"] = "0500";
    m_time_zone["HMT"] = "0500";
    m_time_zone["MAWT"] = "0500";
    m_time_zone["MVT"] = "0500";
    m_time_zone["ORAT"] = "0500";
    m_time_zone["PKT"] = "0500";
    m_time_zone["TFT"] = "0500";
    m_time_zone["TJT"] = "0500";
    m_time_zone["TMT"] = "0500";
    m_time_zone["UZT"] = "0500";
    m_time_zone["YEKT"] = "0500";
    m_time_zone["IST"] = "0530";
    m_time_zone["SLST"] = "0530";
    m_time_zone["NPT"] = "0545";
    m_time_zone["ALMT"] = "0600";
    m_time_zone["BIOT"] = "0600";
    m_time_zone["BST"] = "0600";
    m_time_zone["BTT"] = "0600";
    m_time_zone["KGT"] = "0600";
    m_time_zone["OMST"] = "0600";
    m_time_zone["VOST"] = "0600";
    m_time_zone["CCT"] = "0630";
    m_time_zone["MMT"] = "0630";
    m_time_zone["ACT"] = "0630";
    m_time_zone["CXT"] = "0700";
    m_time_zone["DAVT"] = "0700";
    m_time_zone["HOVT"] = "0700";
    m_time_zone["ICT"] = "0700";
    m_time_zone["KRAT"] = "0700";
    m_time_zone["NOVT"] = "0700";
    m_time_zone["THA"] = "0700";
    m_time_zone["WIT"] = "0700";
    m_time_zone["AWST"] = "0800";
    m_time_zone["BDT"] = "0800";
    m_time_zone["CHOT"] = "0800";
    m_time_zone["CIT"] = "0800";
    m_time_zone["CST"] = "0800";
    m_time_zone["CT"] = "0800";
    m_time_zone["HKT"] = "0800";
    m_time_zone["HOVST"] = "0800";
    m_time_zone["IRKT"] = "0800";
    m_time_zone["MST"] = "0800";
    m_time_zone["MYT"] = "0800";
    m_time_zone["PHT"] = "0800";
    m_time_zone["PST"] = "0800";
    m_time_zone["SGT"] = "0800";
    m_time_zone["SST"] = "0800";
    m_time_zone["ULAT"] = "0800";
    m_time_zone["WST"] = "0800";
    m_time_zone["ACWST"] = "0845";
    m_time_zone["CWST"] = "0845";
    m_time_zone["CHOST"] = "0900";
    m_time_zone["EIT"] = "0900";
    m_time_zone["JST"] = "0900";
    m_time_zone["KST"] = "0900";
    m_time_zone["TLT"] = "0900";
    m_time_zone["ULAST"] = "0900";
    m_time_zone["YAKT"] = "0900";
    m_time_zone["ACST"] = "0930";
    m_time_zone["AEST"] = "1000";
    m_time_zone["CHST"] = "1000";
    m_time_zone["CHUT"] = "1000";
    m_time_zone["DDUT"] = "1000";
    m_time_zone["PGT"] = "1000";
    m_time_zone["VLAT"] = "1000";
    m_time_zone["ACDT"] = "1030";
    m_time_zone["LHST"] = "1030";
    m_time_zone["AEDT"] = "1100";
    m_time_zone["BST"] = "1100";
    m_time_zone["KOST"] = "1100";
    m_time_zone["LHST"] = "1100";
    m_time_zone["MIST"] = "1100";
    m_time_zone["NCT"] = "1100";
    m_time_zone["NFT"] = "1100";
    m_time_zone["PONT"] = "1100";
    m_time_zone["SAKT"] = "1100";
    m_time_zone["SBT"] = "1100";
    m_time_zone["SRET"] = "1100";
    m_time_zone["VUT"] = "1100";
    m_time_zone["ANAT"] = "1200";
    m_time_zone["FJT"] = "1200";
    m_time_zone["GILT"] = "1200";
    m_time_zone["MAGT"] = "1200";
    m_time_zone["MHT"] = "1200";
    m_time_zone["NZST"] = "1200";
    m_time_zone["PETT"] = "1200";
    m_time_zone["TVT"] = "1200";
    m_time_zone["WAKT"] = "1200";
    m_time_zone["CHAST"] = "1245";
    m_time_zone["NZDT"] = "1300";
    m_time_zone["PHOT"] = "1300";
    m_time_zone["TKT"] = "1300";
    m_time_zone["TOT"] = "1300";
    m_time_zone["CHADT"] = "1345";
    m_time_zone["LINT"] = "1400";
    m_time_zone["AZOST"] = "0000";
    m_time_zone["EGST"] = "0000";
    m_time_zone["GMT"] = "0000";
    m_time_zone["UTC"] = "0000";
    m_time_zone["WET"] = "0000";
    m_time_zone["AZOT"] = "-0100";
    m_time_zone["CVT"] = "-0100";
    m_time_zone["EGT"] = "-0100";
    m_time_zone["BRST"] = "-0200";
    m_time_zone["FNT"] = "-0200";
    m_time_zone["GST"] = "-0200";
    m_time_zone["PMDT"] = "-0200";
    m_time_zone["UYST"] = "-0200";
    m_time_zone["NDT"] = "-0230";
    m_time_zone["ADT"] = "-0300";
    m_time_zone["AMST"] = "-0300";
    m_time_zone["ART"] = "-0300";
    m_time_zone["BRT"] = "-0300";
    m_time_zone["CLST"] = "-0300";
    m_time_zone["FKST"] = "-0300";
    m_time_zone["GFT"] = "-0300";
    m_time_zone["PMST"] = "-0300";
    m_time_zone["PYST"] = "-0300";
    m_time_zone["ROTT"] = "-0300";
    m_time_zone["SRT"] = "-0300";
    m_time_zone["UYT"] = "-0300";
    m_time_zone["NST"] = "-0330";
    m_time_zone["NT"] = "-0330";
    m_time_zone["AMT"] = "-0400";
    m_time_zone["AST"] = "-0400";
    m_time_zone["BOT"] = "-0400";
    m_time_zone["CDT"] = "-0400";
    m_time_zone["CLT"] = "-0400";
    m_time_zone["COST"] = "-0400";
    m_time_zone["ECT"] = "-0400";
    m_time_zone["EDT"] = "-0400";
    m_time_zone["FKT"] = "-0400";
    m_time_zone["GYT"] = "-0400";
    m_time_zone["PYT"] = "-0400";
    m_time_zone["VET"] = "-0400";
    m_time_zone["ACT"] = "-0500";
    m_time_zone["CDT"] = "-0500";
    m_time_zone["COT"] = "-0500";
    m_time_zone["CST"] = "-0500";
    m_time_zone["EASST"] = "-0500";
    m_time_zone["ECT"] = "-0500";
    m_time_zone["EST"] = "-0500";
    m_time_zone["PET"] = "-0500";
    m_time_zone["CST"] = "-0600";
    m_time_zone["EAST"] = "-0600";
    m_time_zone["GALT"] = "-0600";
    m_time_zone["MDT"] = "-0600";
    m_time_zone["MST"] = "-0700";
    m_time_zone["PDT"] = "-0700";
    m_time_zone["AKDT"] = "-0800";
    m_time_zone["CIST"] = "-0800";
    m_time_zone["PST"] = "-0800";
    m_time_zone["AKST"] = "-0900";
    m_time_zone["GAMT"] = "-0900";
    m_time_zone["GIT"] = "-0900";
    m_time_zone["HDT"] = "-0900";
    m_time_zone["MART"] = "-0930";
    m_time_zone["MIT"] = "-0930";
    m_time_zone["CKT"] = "-0100";
    m_time_zone["HST"] = "-0100";
    m_time_zone["SDT"] = "-0100";
    m_time_zone["TAHT"] = "-0100";
    m_time_zone["NUT"] = "-1100";
    m_time_zone["SST"] = "-1100";
    m_time_zone["BIT"] = "-1200";
    m_time_zone["IDLW"] = "-1200";
}

std::string Date::getTodayDate() {
    time_t now = time(0);
    tm *local_time = localtime(&now);
    std::string date;
    switch (local_time->tm_wday) {
        case 0: date.append("Sun, "); break;
        case 1: date.append("Mon, "); break;
        case 2: date.append("Tue, "); break;
        case 3: date.append("Wed, "); break;
        case 4: date.append("Thu, "); break;
        case 5: date.append("Fri, "); break;
        case 6: date.append("Sat, "); break;
    }
    std::stringstream ss;
    ss << local_time->tm_mday;
    date.append(ss.str());
    ss.str("");
    date.append(" ");
    switch (local_time->tm_mon) {
        case 0: date.append("Jan "); break;
        case 1: date.append("Feb "); break;
        case 2: date.append("Mar "); break;
        case 3: date.append("Apr "); break;
        case 4: date.append("May "); break;
        case 5: date.append("Jun "); break;
        case 6: date.append("Jul "); break;
        case 7: date.append("Aug "); break;
        case 8: date.append("Sep "); break;
        case 9: date.append("Oct "); break;
        case 10: date.append("Nov "); break;
        case 11: date.append("Dec "); break;
    }
    ss << local_time->tm_year + 1900;
    date.append(ss.str());
    ss.str("");
    date.append(" ");
    ss << 1 + local_time->tm_hour;
    date.append(ss.str());
    ss.str("");
    date.append(":");
    ss << 1 + local_time->tm_min;
    date.append(ss.str());
    ss.str("");
    date.append(":");
    ss << 1 + local_time->tm_sec;
    date.append(ss.str());
    ss.str("");
    date.append(" ");
    std::string zone = local_time->tm_zone;
    date.append(m_time_zone.find(zone)->second);
    date.append(" ");
    return date;
}

std::string Date::nowTime() {
    std::stringstream ss;
    time_t t = time(0); // get time now
    tm *now = localtime(&t);
    struct timeval tv;
    gettimeofday(&tv, NULL);
    ss << (now->tm_year + 1900) << '-' << (now->tm_mon + 1) << '-' << now->tm_mday << " " << now->tm_hour
    << ":" << now->tm_min << ":" << now->tm_sec << "." << tv.tv_usec << " " << now->tm_zone << " ";
    return ss.str();
}

long Date::timestamp() {
    return (std::chrono::time_point_cast<std::chrono::microseconds>(std::chrono::system_clock::now())).time_since_epoch().count();
}

NtpClient Date::ntpTimestamp() {
    return NtpClient::instance("us.pool.ntp.org", 123);
}

