/*
 * EDA4U - Professional EDA for everyone!
 * Copyright (C) 2013 Urban Bruhin
 * http://eda4u.ubruhin.ch/
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <QtCore>
#include <QDomElement>
#include "../../common/exceptions.h"
#include "gencompsignalinstance.h"
#include "gencompinstance.h"
#include "circuit.h"
#include "netsignal.h"
#include "../../library/genericcomponent.h"
#include "../erc/ercmsg.h"

namespace project {

/*****************************************************************************************
 *  Constructors / Destructor
 ****************************************************************************************/

GenCompSignalInstance::GenCompSignalInstance(Circuit& circuit, GenCompInstance& genCompInstance,
                                             const QDomElement& domElement) throw (Exception) :
    QObject(0), mCircuit(circuit), mGenCompInstance(genCompInstance), mDomElement(domElement),
    mGenCompSignal(nullptr), mNetSignal(nullptr), mAddedToCircuit(false)
{
    // read attributes
    QString genCompSignalUuid = mDomElement.attribute("comp_signal");
    mGenCompSignal = mGenCompInstance.getGenComp().getSignalByUuid(genCompSignalUuid);
    if(!mGenCompSignal)
    {
        throw RuntimeError(__FILE__, __LINE__, genCompSignalUuid, QString(
            tr("Invalid component signal UUID: \"%1\"")).arg(genCompSignalUuid));
    }
    QString netsignalUuid = mDomElement.attribute("netsignal");
    if (!netsignalUuid.isEmpty())
    {
        mNetSignal = mCircuit.getNetSignalByUuid(netsignalUuid);
        if(!mNetSignal)
        {
            throw RuntimeError(__FILE__, __LINE__, netsignalUuid,
                QString(tr("Invalid netsignal UUID: \"%1\"")).arg(netsignalUuid));
        }
    }

    // create ERC messages
    mErcMsgUnconnectedRequiredSignal.reset(new ErcMsg(mCircuit.getProject(), *this,
        QString("%1/%2").arg(mGenCompInstance.getUuid().toString()).arg(mGenCompSignal->getUuid().toString()),
        "UnconnectedRequiredSignal", ErcMsg::ErcMsgType_t::CircuitError,
        QString(tr("Unconnected component signal: \"%1\" from \"%2\""))
        .arg(mGenCompSignal->getName()).arg(mGenCompInstance.getName())));
}

GenCompSignalInstance::~GenCompSignalInstance() noexcept
{
    Q_ASSERT(!mAddedToCircuit);
    Q_ASSERT(mSymbolPinInstances.isEmpty());
}

/*****************************************************************************************
 *  Setters
 ****************************************************************************************/

void GenCompSignalInstance::setNetSignal(NetSignal* netsignal) throw (Exception)
{
    if (!mAddedToCircuit)
        throw LogicError(__FILE__, __LINE__);

    if (mNetSignal)
        mNetSignal->unregisterGenCompSignal(this);

    if (netsignal)
        netsignal->registerGenCompSignal(this);

    if (netsignal)
        mDomElement.setAttribute("netsignal", netsignal->getUuid().toString());
    else
        mDomElement.setAttribute("netsignal", "");

    mNetSignal = netsignal;
    mErcMsgUnconnectedRequiredSignal->setVisible((!mNetSignal) && (mGenCompSignal->isRequired()));
}

/*****************************************************************************************
 *  General Methods
 ****************************************************************************************/

void GenCompSignalInstance::registerSymbolPinInstance(SymbolPinInstance* pin) throw (Exception)
{
    Q_CHECK_PTR(pin);

    if (!mAddedToCircuit)
        throw LogicError(__FILE__, __LINE__);
    if (mSymbolPinInstances.contains(pin))
        throw LogicError(__FILE__, __LINE__);

    mSymbolPinInstances.append(pin);
}

void GenCompSignalInstance::unregisterSymbolPinInstance(SymbolPinInstance* pin) throw (Exception)
{
    Q_CHECK_PTR(pin);

    if (!mAddedToCircuit)
        throw LogicError(__FILE__, __LINE__);
    if (!mSymbolPinInstances.contains(pin))
        throw LogicError(__FILE__, __LINE__);

    mSymbolPinInstances.removeAll(pin);
}

void GenCompSignalInstance::addToCircuit() throw (Exception)
{
    if (!mSymbolPinInstances.isEmpty())
        throw LogicError(__FILE__, __LINE__);

    if (mNetSignal)
        mNetSignal->registerGenCompSignal(this);

    mAddedToCircuit = true;
    mErcMsgUnconnectedRequiredSignal->setVisible((!mNetSignal) && (mGenCompSignal->isRequired()));
}

void GenCompSignalInstance::removeFromCircuit() throw (Exception)
{
    if (!mSymbolPinInstances.isEmpty())
        throw LogicError(__FILE__, __LINE__);

    if (mNetSignal)
        mNetSignal->unregisterGenCompSignal(this);

    mAddedToCircuit = false;
    mErcMsgUnconnectedRequiredSignal->setVisible(false);
}

/*****************************************************************************************
 *  End of File
 ****************************************************************************************/

} // namespace project
