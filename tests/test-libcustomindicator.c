
#include <glib.h>
#include <glib-object.h>

#include <libcustomindicator/custom-indicator.h>

void
test_libcustomindicator_init_with_props (void)
{
	CustomIndicator * ci = CUSTOM_INDICATOR(g_object_new(CUSTOM_INDICATOR_TYPE, 
	                                                     "id", "my-id",
	                                                     "category", CUSTOM_INDICATOR_CATEGORY_APPLICATION_STATUS,
	                                                     "status", CUSTOM_INDICATOR_STATUS_ON,
	                                                     "icon-name", "my-name",
	                                                     "attention-icon-name", "my-attention-name",
	                                                     NULL));
	g_assert(ci != NULL);

	g_assert(!g_strcmp0("my-id", custom_indicator_get_id(ci)));
	g_assert(!g_strcmp0("my-name", custom_indicator_get_icon(ci)));
	g_assert(!g_strcmp0("my-attention-name", custom_indicator_get_attention_icon(ci)));
	g_assert(custom_indicator_get_status(ci) == CUSTOM_INDICATOR_STATUS_ON);
	g_assert(custom_indicator_get_category(ci) == CUSTOM_INDICATOR_CATEGORY_APPLICATION_STATUS);

	g_object_unref(G_OBJECT(ci));
	return;
}

void
test_libcustomindicator_init (void)
{
	CustomIndicator * ci = CUSTOM_INDICATOR(g_object_new(CUSTOM_INDICATOR_TYPE, NULL));
	g_assert(ci != NULL);
	g_object_unref(G_OBJECT(ci));
	return;
}

void
test_libcustomindicator_props_suite (void)
{
	g_test_add_func ("/indicator-custom/libcustomindicator/init",        test_libcustomindicator_init);
	g_test_add_func ("/indicator-custom/libcustomindicator/init_props",  test_libcustomindicator_init_with_props);


	return;
}

gint
main (gint argc, gchar * argv[])
{
	g_type_init();
	g_test_init(&argc, &argv, NULL);

	/* Test suites */
	test_libcustomindicator_props_suite();


	return g_test_run ();
}
