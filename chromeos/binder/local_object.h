// Copyright 2015 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef CHROMEOS_BINDER_LOCAL_OBJECT_H_
#define CHROMEOS_BINDER_LOCAL_OBJECT_H_

#include "base/macros.h"
#include "base/memory/scoped_ptr.h"
#include "chromeos/binder/object.h"
#include "chromeos/chromeos_export.h"

namespace binder {

// Object living in the current process.
class CHROMEOS_EXPORT LocalObject : public Object {
 public:
  // Inherit this interface to implement transaction.
  class TransactionHandler {
   public:
    virtual ~TransactionHandler() {}

    // Called when LocalObject::Transact() is called.
    virtual scoped_ptr<TransactionData> OnTransact(
        CommandBroker* command_broker,
        const TransactionData& data) = 0;
  };

  explicit LocalObject(scoped_ptr<TransactionHandler> handler);

  // Object override:
  Type GetType() const override;
  bool Transact(CommandBroker* command_broker,
                const TransactionData& data,
                scoped_ptr<TransactionData>* reply) override;

 protected:
  ~LocalObject() override;

 private:
  scoped_ptr<TransactionHandler> handler_;

  DISALLOW_COPY_AND_ASSIGN(LocalObject);
};

}  // namespace binder

#endif  // CHROMEOS_BINDER_LOCAL_OBJECT_H_
