/*
 * File: IDataSource.h
 * Description:
 *   Interface abstraction for data sources. Intended for testability outside of the
 *   Arduino environment.
 * Author: Mooneer Salem <mooneer@gmail.com>
 * License: New BSD License
 */

#ifndef I_DATASOURCE_H
#define I_DATASOURCE_H

class IDataSource
{
public:
    virtual ~IDataSource() { }

    /*
     * Returns whether there is data available to be read.
     */
    virtual bool available() = 0;
    
    /*
     * Returns one character from the data source.
     */
    virtual int read() = 0;
};

#endif // I_DATASOURCE_H
