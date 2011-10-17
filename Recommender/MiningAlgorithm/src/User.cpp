/* Copyright (C) Paulo Muniz de Ávila <avilapm@gmail.com> 2009
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "../include/User.h"

namespace br {
    namespace ufscar {
        namespace lince {
            namespace ginga {
                namespace recommender {

                    User::User() {
                    }

                    User::~User() {
                    }

                    int User::getCode() {
                        return User::code;
                    }

                    void User::setCode(int setcode) {
                        User::code = setcode;
                    }

                    string User::getKey() {
                        return User::keycode;
                    }

                    void User::setKey(string setKey) {
                        User::keycode = setKey;
                    }

                    string User::getDocid() {
                        return User::doc_id;
                    }

                    void User::setDocid(string setdocid) {
                        User::doc_id = setdocid;
                    }

                    string User::getFilename() {
                        return User::filename;
                    }

                    void User::setFilename(string setfilename) {
                        User::filename = setfilename;
                    }

                    string User::getHour() {
                        return User::hour;
                    }

                    void User::setHour(string sethour) {
                        User::hour = sethour;
                    }

                    string User::getMinute() {
                        return User::minute;
                    }

                    void User::setMinute(string setminute) {
                        User::minute = setminute;
                    }

                    string User::getSecond() {
                        return User::second;
                    }

                    void User::setSecond(string setsecond) {
                        User::second = setsecond;
                    }

                    string User::getDay() {
                        return User::day;
                    }

                    void User::setDay(string setday) {
                        User::day = setday;
                    }

                    string User::getMonth() {
                        return User::month;
                    }

                    void User::setMonth(string setmonth) {
                        User::month = setmonth;
                    }

                    string User::getYear() {
                        return User::year;
                    }

                    void User::setYear(string setyear) {
                        User::year = setyear;
                    }

                    string User::getWeekDay() {
                        return User::dayweek;
                    }

                    void User::setWeekDay(string setweekday) {
                        User::dayweek = setweekday;
                    }

                    double User::getTimeDuration() {
                        return User::time;
                    }

                    void User::setTimeDuration(double settimeduration) {
                        User::time = settimeduration;
                    }

                    string User::getFormatDate() {
                        return User::getYear() + "-" + User::getMonth() + "-" + User::getDay();
                    }

                    string User::getFormatTime() {
                        return User::getHour() + ":" + User::getMinute() + ":" + User::getSecond();
                    }

                }
            }
        }
    }
}


