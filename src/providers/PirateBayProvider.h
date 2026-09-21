
#include "providers/TorrentProvider.h"

class PirateBayProvider : public TorrentProvider {

    public:

        explicit PirateBayProvider(QObject* parent = nullptr);

        void search(
            const unsigned int searchId,
            const QString& query
        ) const override;

};
