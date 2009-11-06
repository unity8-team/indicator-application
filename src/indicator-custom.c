
#include <glib.h>
#include <glib-object.h>

#include <dbus/dbus-glib.h>

#include <libindicator/indicator.h>
#include <libindicator/indicator-object.h>
#include <libindicator/indicator-service-manager.h>
#include "dbus-shared.h"


#define INDICATOR_CUSTOM_TYPE            (indicator_custom_get_type ())
#define INDICATOR_CUSTOM(obj)            (G_TYPE_CHECK_INSTANCE_CAST ((obj), INDICATOR_CUSTOM_TYPE, IndicatorCustom))
#define INDICATOR_CUSTOM_CLASS(klass)    (G_TYPE_CHECK_CLASS_CAST ((klass), INDICATOR_CUSTOM_TYPE, IndicatorCustomClass))
#define IS_INDICATOR_CUSTOM(obj)         (G_TYPE_CHECK_INSTANCE_TYPE ((obj), INDICATOR_CUSTOM_TYPE))
#define IS_INDICATOR_CUSTOM_CLASS(klass) (G_TYPE_CHECK_CLASS_TYPE ((klass), INDICATOR_CUSTOM_TYPE))
#define INDICATOR_CUSTOM_GET_CLASS(obj)  (G_TYPE_INSTANCE_GET_CLASS ((obj), INDICATOR_CUSTOM_TYPE, IndicatorCustomClass))

typedef struct _IndicatorCustom      IndicatorCustom;
typedef struct _IndicatorCustomClass IndicatorCustomClass;

struct _IndicatorCustomClass {
	IndicatorObjectClass parent_class;
};

struct _IndicatorCustom {
	IndicatorObject parent;
};

GType indicator_custom_get_type (void);

INDICATOR_SET_VERSION
INDICATOR_SET_TYPE(INDICATOR_CUSTOM_TYPE)


#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

typedef struct _IndicatorCustomPrivate IndicatorCustomPrivate;

struct _IndicatorCustomPrivate
{
	IndicatorServiceManager * sm;
	DBusGConnection * bus;
	DBusGProxy * service_proxy;
};

#define INDICATOR_CUSTOM_GET_PRIVATE(o) \
(G_TYPE_INSTANCE_GET_PRIVATE ((o), INDICATOR_CUSTOM_TYPE, IndicatorCustomPrivate))

static void indicator_custom_class_init (IndicatorCustomClass *klass);
static void indicator_custom_init       (IndicatorCustom *self);
static void indicator_custom_dispose    (GObject *object);
static void indicator_custom_finalize   (GObject *object);
GList * get_entries (IndicatorObject * io);
void connected (IndicatorServiceManager * sm, gboolean connected, IndicatorCustom * custom);

G_DEFINE_TYPE (IndicatorCustom, indicator_custom, INDICATOR_OBJECT_TYPE);

static void
indicator_custom_class_init (IndicatorCustomClass *klass)
{
	GObjectClass *object_class = G_OBJECT_CLASS (klass);

	g_type_class_add_private (klass, sizeof (IndicatorCustomPrivate));

	object_class->dispose = indicator_custom_dispose;
	object_class->finalize = indicator_custom_finalize;

	IndicatorObjectClass * io_class = INDICATOR_OBJECT_CLASS(klass);

	io_class->get_entries = get_entries;

	return;
}

static void
indicator_custom_init (IndicatorCustom *self)
{
	IndicatorCustomPrivate * priv = INDICATOR_CUSTOM_GET_PRIVATE(self);

	/* These are built in the connection phase */
	priv->bus = NULL;
	priv->service_proxy = NULL;

	priv->sm = indicator_service_manager_new(INDICATOR_CUSTOM_DBUS_ADDR);	
	g_signal_connect(G_OBJECT(priv->sm), INDICATOR_SERVICE_MANAGER_SIGNAL_CONNECTION_CHANGE, G_CALLBACK(connected), self);


	return;
}

static void
indicator_custom_dispose (GObject *object)
{
	IndicatorCustomPrivate * priv = INDICATOR_CUSTOM_GET_PRIVATE(object);

	if (priv->sm != NULL) {
		g_object_unref(priv->sm);
		priv->sm = NULL;
	}

	if (priv->bus != NULL) {
		/* We're not incrementing the ref count on this one. */
		priv->bus = NULL;
	}

	if (priv->service_proxy != NULL) {
		g_object_unref(G_OBJECT(priv->service_proxy));
		priv->service_proxy = NULL;
	}

	G_OBJECT_CLASS (indicator_custom_parent_class)->dispose (object);
	return;
}

static void
indicator_custom_finalize (GObject *object)
{

	G_OBJECT_CLASS (indicator_custom_parent_class)->finalize (object);
	return;
}

void
connected (IndicatorServiceManager * sm, gboolean connected, IndicatorCustom * custom)
{
	IndicatorCustomPrivate * priv = INDICATOR_CUSTOM_GET_PRIVATE(custom);
	g_debug("Connected to Custom Indicator Service.");

	GError * error = NULL;

	if (priv->bus == NULL) {
		priv->bus = dbus_g_bus_get(DBUS_BUS_SESSION, &error);

		if (error != NULL) {
			g_error("Unable to get session bus: %s", error->message);
			g_error_free(error);
			return;
		}
	}

	priv->service_proxy = dbus_g_proxy_new_for_name_owner(priv->bus,
	                                                      INDICATOR_CUSTOM_DBUS_ADDR,
	                                                      INDICATOR_CUSTOM_DBUS_OBJ,
	                                                      INDICATOR_CUSTOM_DBUS_IFACE,
	                                                      &error);

	return;
}

GList *
get_entries (IndicatorObject * io)
{

	return NULL;
}
